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

icmp_packet *fill_packethdr(int payload_size)
{
    icmp_packet *pack = ft_calloc(1, sizeof(icmp_packet) + payload_size);
    if (!pack)
        return NULL;

    pack->icmp.type = ICMP_ECHO;
    pack->icmp.code = 0;
    pack->icmp.un.echo.id = getpid();

    for (int i = 0; i < payload_size; i++)
        pack->garbage[i] = 0x10 + (i % 112);

    return pack;
}

int ping_loop(int sockfd, char *arg, struct sockaddr_in *host_addr, packetvalue *progval, dnsinfo *host, ping_flags *flags)
{
	struct timeval	packet_start, packet_end, ping_start, ping_end;
	uint16_t		seq = 0;
	char			buff[1024];
	icmp_packet		*pack = fill_packethdr(progval->payload_size);
	if (!pack)
		return (0);

	int pack_size = progval->payload_size + (int)(sizeof(struct icmphdr));

	if (flags->v_flag)
		printf("PING %s (%s): %ld data bytes, id 0x%x = %d\n", arg, host->host_addr, progval->payload_size, pack->icmp.un.echo.id, pack->icmp.un.echo.id);
	else
		printf("PING %s (%s): %ld data bytes\n", arg, host->host_addr, progval->payload_size);

	unsigned long long nbr_packet = 0;
	while (g_sig == 0 && (progval->nbr_max_packet == 0 || nbr_packet < progval->nbr_max_packet))
	{
		if (g_alrm)
		{
			g_alrm = alarm(1);
			if (!nbr_packet)
				gettimeofday(&ping_start, NULL);
			nbr_packet++;
			pack->icmp.un.echo.sequence = seq;
			pack->icmp.checksum = 0;
			pack->icmp.checksum = checksum(pack, pack_size);

			gettimeofday(&packet_start, NULL);
			ssize_t sto = sendto(sockfd, pack, pack_size, 0, (const struct sockaddr *)host_addr, sizeof(*host_addr));
			if (sto == -1)
				return (free(pack), fprintf(stderr, "%s\n", strerror(errno)), 0);

			struct sockaddr_in	reply;
			socklen_t			rlen = sizeof(reply);

			ssize_t rfm = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&reply, &rlen);
			if (rfm == -1)
			{
				progval->pack_lost++;
				gettimeofday(&ping_end, NULL);
				seq++;
				continue;
			}
			gettimeofday(&packet_end, NULL);
			float packet_time = (packet_end.tv_sec - packet_start.tv_sec) * 1000 + ((packet_end.tv_usec - packet_start.tv_usec) / 1000.0);
			if (flags->D_flag)
				printf("[%ld.%ld] ", packet_start.tv_sec, packet_start.tv_usec);
			// if (flags->n_flag)
			// 	printf("%d bytes from %s: ", (int)sto, host->host_addr);
			// else
			printf("%d bytes from %s: ", (int)sto, host->host_addr);

			// if (flags->v_flag)
			// 	printf("icmp_seq=%d ident=%d ttl=%d time=%.3f ms", seq, pack->icmp.un.echo.id, ((struct iphdr *)buff)->ttl, packet_time);
			// else
			printf("icmp_seq=%d ttl=%d time=%.3f ms", seq, ((struct iphdr *)buff)->ttl, packet_time);
			printf("\n");
			progval->ptime_total += packet_time;
			progval->ptime_total_2 += packet_time * packet_time;
			progval->ptime_max = (packet_time > progval->ptime_max) ? packet_time : progval->ptime_max;
			progval->ptime_min = (packet_time < progval->ptime_max || progval->ptime_min == 0) ? packet_time : progval->ptime_min;
			gettimeofday(&ping_end, NULL);
			seq++;
		}
	}
	progval->pack_total = seq;
	progval->ptime_avg = progval->ptime_total / seq;
	progval->total_time = (ping_end.tv_sec - ping_start.tv_sec) * 1000 + ((ping_end.tv_usec - ping_start.tv_usec) / 1000.0);
	progval->ptime_mdev = sqrtf(progval->ptime_total_2 / seq - powf(progval->ptime_avg, 2));
	free(pack);
	return (1);
}