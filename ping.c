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

static void	fill_packethdr(icmp_packet *pack)
{
	ft_memset(pack, 0, sizeof(pack));
	pack->icmp.type = ICMP_ECHO;		//ne bouge pas a chaque boucle
	pack->icmp.code = 0;				//ne bouge pas a chaque boucle
	pack->icmp.un.echo.id = getpid();	//ne bouge pas a chaque boucle

	for (int i = 0; i < 56; i++)
		pack->garbage[i] = 0x10 + i;

	return ;
}

int ping_loop(int sockfd, char *arg, struct sockaddr_in *host_addr, packetvalue *progval, dnsinfo *host, dnsinfo *l_host, ping_flags *flags)
{
	struct timeval	packet_start, packet_end, ping_start, ping_end;
	int				seq = 0;
	int addr_diff = ft_strncmp(l_host->host_addr, host->host_addr, 21);
	char buff[1024];
	icmp_packet	pack;
	fill_packethdr(&pack);
	int first = 0;

	printf("PING %s (%s) %d(%d) bytes of data.\n", arg, l_host->host_addr, (int)(sizeof(pack.garbage)), (int)(sizeof(pack) + sizeof(struct iphdr)));
	while (g_sig == 0)
	{
		if (g_alrm)
		{
			if (!first++)
				gettimeofday(&ping_start, NULL);
			pack.icmp.un.echo.sequence = seq++;
			pack.icmp.checksum = 0;
			pack.icmp.checksum = checksum(&pack, sizeof(pack));

			gettimeofday(&packet_start, NULL);
			ssize_t sto = sendto(sockfd, &pack, sizeof(pack), 0, (const struct sockaddr *)host_addr, sizeof(*host_addr));
			if (sto == -1)
				return (fprintf(stderr, "%s\n", strerror(errno)), 0);

			struct sockaddr_in	reply;
			socklen_t			rlen = sizeof(reply);

			ssize_t rfm = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&reply, &rlen);
			if (rfm == -1)
				return (fprintf(stderr, "%s\n", strerror(errno)), 0);
			gettimeofday(&packet_end, NULL);

			struct iphdr	*replyIp = (struct iphdr *)buff;
			struct icmphdr	*replyIcmp = (struct icmphdr *)(buff + sizeof(struct iphdr));
			if (replyIcmp->type != 0)
				progval->pack_lost++;
			float packet_time = (packet_end.tv_sec - packet_start.tv_sec) * 1000 + ((packet_end.tv_usec - packet_start.tv_usec) / 1000.0);
			if (flags->v_flag)
				printf("%d bytes from %s (%s): icmp_seq=%d ident=%d ttl=%d time=%.2f ms", (int)sto, host->domain_name, host->host_addr, seq, pack.icmp.un.echo.id, replyIp->ttl, packet_time);
			else
				printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms", (int)sto, host->domain_name, host->host_addr, seq, replyIp->ttl, packet_time);
			addr_diff != 0 ? printf(" (DIFFERENT ADDRESS!)\n") : printf("\n");
			progval->ptime_total += packet_time;
			progval->ptime_max = (packet_time > progval->ptime_max) ? packet_time : progval->ptime_max;
			progval->ptime_min = (packet_time < progval->ptime_max || progval->ptime_min == 0) ? packet_time : progval->ptime_min;
			alarm(1);
			g_alrm = 0;
			gettimeofday(&ping_end, NULL);
		}
	}
	progval->pack_total = seq;
	progval->ptime_avg = progval->ptime_total / seq;
	progval->total_time = (ping_end.tv_sec - ping_start.tv_sec) * 1000 + ((ping_end.tv_usec - ping_start.tv_usec) / 1000.0);
	return (1);
}