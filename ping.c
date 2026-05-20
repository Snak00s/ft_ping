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
	pack->icmp.type = ICMP_ECHO;		//ne bouge pas a chaque boucle
	pack->icmp.code = 0;				//ne bouge pas a chaque boucle
	pack->icmp.un.echo.id = getpid();	//ne bouge pas a chaque boucle

	for (int i = 0; i < 56; i++)
		pack->garbage[i] = 0x10 + i;

	return ;
}

int ping_loop(int sockfd, char *arg, struct sockaddr_in *host_addr, packetvalue *progval, dnsinfo *host, ping_flags *flags)
{
	struct timeval	packet_start, packet_end;
	int				seq = 0;
	char buff[1024];
	icmp_packet	pack;
	memset(&pack, 0, sizeof(pack));
	fill_packethdr(&pack);

	printf("PING %s (%s) %d(%d) bytes of data.\n", arg, host->host_addr, (int)(sizeof(pack.garbage)), (int)(sizeof(pack) + sizeof(struct iphdr)));
	while (g_sig == 0)
	{
		pack.icmp.un.echo.sequence = seq++;
		pack.icmp.checksum = 0;
		pack.icmp.checksum = checksum(&pack, sizeof(pack));

		// printf("send : type = %d code = %d, checksum = %d\n", icmp.type, icmp.code, icmp.checksum);
		gettimeofday(&packet_start, NULL);
		ssize_t sto = sendto(sockfd, &pack, sizeof(pack), 0, (const struct sockaddr *)host_addr, sizeof(*host_addr));
		if (sto == -1)
		{
			// perror("sendto");
			fprintf(stderr, "%s\n", strerror(errno));
			return (0);
		}
		struct sockaddr_in	reply;
		socklen_t			rlen = sizeof(reply);
		ssize_t rfm = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&reply, &rlen);
		if (rfm == -1)
		{
			// perror("recvfrom");
			fprintf(stderr, "%s\n", strerror(errno));
			return (0);
		}
		gettimeofday(&packet_end, NULL);
		// printf("size sended = %lu\nsize received = %lu\n", sto, rfm);
		struct iphdr	*replyIp = (struct iphdr *)buff;
		struct icmphdr	*replyIcmp = (struct icmphdr *)(buff + sizeof(struct iphdr));
		// printf("reply icmp : type = %d code = %d, checksum = %d\n", replyIcmp->type, replyIcmp->code, replyIcmp->checksum);
		if (replyIcmp->type != 0)
			progval->pack_lost++;
		float packet_time = (packet_end.tv_sec - packet_start.tv_sec) + ((packet_end.tv_usec - packet_start.tv_usec) / 1000.0);
		if (flags->v_flag)
			printf("%d bytes from %s (%s): icmp_seq=%d ident=%d ttl=%d time=%.2f ms\n", (int)sto, host->domain_name, host->host_addr, seq, pack.icmp.un.echo.id, replyIp->ttl, packet_time);
		else
			printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n", (int)sto, host->domain_name, host->host_addr, seq, replyIp->ttl, packet_time);
		progval->ptime_total += packet_time;
		progval->ptime_max = (packet_time > progval->ptime_max) ? packet_time : progval->ptime_max;
		progval->ptime_min = (packet_time < progval->ptime_max || progval->ptime_min == 0) ? packet_time : progval->ptime_min;
		sleep(1);
	}
	progval->pack_total = seq;
	progval->ptime_avg = progval->ptime_total / seq;
	return (1);
}