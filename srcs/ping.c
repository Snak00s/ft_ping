#include"ping.h"

static uint16_t checksum(void *addr, int size)
{
	uint32_t		sum = 0;
	unsigned short	*ptr = addr;

	while (size > 1)
	{
		sum += *ptr++;
		size -= 2;
	}
	if (size > 0)
		sum += *(unsigned char *)ptr;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (~sum);
}

static void	swap_char(char *c1, char *c2)
{
	char temp = *c2;

	*c2 = *c1;
	*c1 = temp;
}

static icmp_packet *fill_packethdr(int payload_size, char *patern)
{
	icmp_packet *pack = ft_calloc(1, sizeof(icmp_packet) + payload_size);
	if (!pack)
		return NULL;

	pack->icmp.type = ICMP_ECHO;
	pack->icmp.code = 0;
	pack->icmp.un.echo.id = getpid();
	if (!patern)
	{
		for (int i = 0; i < payload_size; i++)
			pack->garbage[i] = 0x10 + (i % 112);
	}
	else
	{
		int 	pat_len = ft_strlen(patern);
		char	hex[3];
		hex[2] = '\0';
		int i = 0;
		if (pat_len > 1)
		{
			pat_len = pat_len > 32 ? 32 : pat_len;
			int j = 0;
			int idx = 0;
			while (i < payload_size)
			{
				hex[idx++] = patern[j++];
				if (patern[j] == '\0')
				{
					j = 0;
					if (pat_len % 2 != 0)
					{
						swap_char(&hex[0], &hex[1]);
						idx++;
					}
				}
				if (idx > 1)
				{
					pack->garbage[i++] = strtol(hex, NULL, 16);
					idx = 0;
					hex[0] = hex[1] = '0';
				}
			}
		}
		else
		{
			hex[0] = '0';
			hex[1] = *patern;
			char hex_char = strtol(hex, NULL, 16);
			while (i < payload_size)
				pack->garbage[i++] = hex_char;
		}
	}
	return (pack);
}

static int	print_result(packet_value *progval, char *hostname)
{
	int rec_packet = progval->pack_total - progval->pack_lost;
	int percent_loss = (progval->pack_lost * 100) / progval->pack_total;
	printf("--- %s ping statistics ---\n%d packets transmitted, %d packets received, %d%% packet loss\n",
			hostname, progval->pack_total, rec_packet, percent_loss);
	if (percent_loss != 100)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", progval->ptime_min, progval->ptime_avg, progval->ptime_max, progval->ptime_mdev);
	return (percent_loss == 100);
}

static int stop_ping_cond(ping_option_value *ping_opt, unsigned long long nbr_packet, float total_time)
{
	if (ping_opt->nbr_max_packet != 0 && nbr_packet >= ping_opt->nbr_max_packet)
		return (0);
	if (ping_opt->max_possible_time != 0 && total_time >= ping_opt->max_possible_time)
		return (0);

	return (1);
}

int ping_loop(int sockfd, struct sockaddr_in *host_addr, dns_info *host, int nbr_dest, ping_option_value *ping_opt, ping_flags *flags, int io_flag)
{
	struct timeval	packet_start, packet_end, ping_start, ping_end;
	char			buff[1024];
	uint16_t		seq = 0;
	int				ret = 0;
	packet_value	progval;
	icmp_packet		*pack = fill_packethdr(ping_opt->payload_size, ping_opt->payload_patern);
	if (!pack)
		return (0);

	int pack_size = ping_opt->payload_size + (int)(sizeof(struct icmphdr));

	int i = 0;
	while (i < nbr_dest)
	{
		ft_memset(&progval, 0, sizeof(progval));
		if (!host[i].ping_started)
		{
			if (flags->v_flag)
				printf("PING %s (%s): %ld data bytes, id 0x%x = %d\n", host[i].argv_dest, host[i].host_addr, ping_opt->payload_size, pack->icmp.un.echo.id, pack->icmp.un.echo.id);
			else
				printf("PING %s (%s): %ld data bytes\n", host[i].argv_dest, host[i].host_addr, ping_opt->payload_size);
			host[i].ping_started = 1;
		}

		unsigned long long nbr_packet = 0;
		seq = 0;
		while (g_sig == 0 && stop_ping_cond(ping_opt, nbr_packet, progval.total_time))
		{
			if (flags->f_flag || g_alrm || ping_opt->preload != 0)
			{
				if (ping_opt->preload != 0)
					ping_opt->preload--;
				else
					g_alrm = alarm(1);

				if (!nbr_packet)
					gettimeofday(&ping_start, NULL);
				nbr_packet++;
				pack->icmp.un.echo.sequence = seq;
				pack->icmp.checksum = 0;
				pack->icmp.checksum = checksum(pack, pack_size);

				gettimeofday(&packet_start, NULL);
				ssize_t sto = sendto(sockfd, pack, pack_size, io_flag, (const struct sockaddr *)&host_addr[i], sizeof(host_addr[i]));
				if (sto == -1)
				{
					free(pack);
					fprintf(stderr, "%s\n", strerror(errno));
					return (0);
				}

				struct sockaddr_in	reply;
				socklen_t			rlen = sizeof(reply);

				ssize_t rfm = recvfrom(sockfd, buff, sizeof(buff), io_flag, (struct sockaddr *)&reply, &rlen);
				if (rfm == -1)
				{
					progval.pack_lost++;
					gettimeofday(&ping_end, NULL);
					seq++;
					continue;
				}
				gettimeofday(&packet_end, NULL);
				float packet_time = (packet_end.tv_sec - packet_start.tv_sec) * 1000 + ((packet_end.tv_usec - packet_start.tv_usec) / 1000.0);
				if (!flags->q_flag && !flags->f_flag)
					printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", (int)sto, host[i].host_addr, seq, ((struct iphdr *)buff)->ttl, packet_time);
				progval.ptime_total += packet_time;
				progval.ptime_total_2 += packet_time * packet_time;
				progval.ptime_max = (packet_time > progval.ptime_max) ? packet_time : progval.ptime_max;
				progval.ptime_min = (packet_time < progval.ptime_min || progval.ptime_min == 0) ? packet_time : progval.ptime_min;
				seq++;
			}
			gettimeofday(&ping_end, NULL);
			progval.total_time = (ping_end.tv_sec - ping_start.tv_sec) + ((ping_end.tv_usec - ping_start.tv_usec) / 1000000.0);
		}
		if (seq == 0)
		{
			progval.pack_total = 1;
			progval.pack_lost = 1;
		}
		else
		{
			progval.pack_total = seq;
			progval.ptime_avg = progval.ptime_total / seq;
			progval.ptime_mdev = sqrtf(progval.ptime_total_2 / seq - powf(progval.ptime_avg, 2));
		}
		for (unsigned int j = 0; j < progval.pack_lost; j++)
			write(1, ".", 1);
		ret = print_result(&progval, host[i].argv_dest);
		i++;
	}
	free(pack);
	return (ret);
}