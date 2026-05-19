#include"ping.h"

int	g_sig = 0;

void	handler(int sig)
{
	(void)sig;
	g_sig = 1;
}

void	signal_handler(void)
{
	struct sigaction	sa;
	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = &handler;
	sigaction(SIGINT, &sa, 0);
}

uint16_t checksum(void *addr, int size)
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


// a tester

void	fill_packethdr(packet *pack)
{
	struct iphdr *ip = &pack->ip;
	struct icmphdr *icmp = &pack->icmp;

	ip->version = 4;
	ip->ihl = 5;
	ip->check = 0;
	ip->protocol = 1;
	ip->id = getpid();

	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = getpid();

	for (int i = 0; i < 56; i++)
		pack->garbage[i] = 0x10 + i;

	return ;
}

// a tester

int ping_loop(int sockfd, char *arg, struct sockaddr_in *host_addr, packetvalue *progval, dnsinfo *host)
{
	struct icmphdr	icmp;
	struct timeval	packet_start, packet_end;
	int				seq = 0;
	char buff[1024];
	// packet	test;

	// memset(&test, 0, sizeof(test));
	// printf("%lu", sizeof(test));

	icmp.type = ICMP_ECHO;			//ne bouge pas a chaque boucle
	icmp.code = 0;					//ne bouge pas a chaque boucle
	icmp.un.echo.id = getpid();		//ne bouge pas a chaque boucle

	printf("PING %s (%s)\n", arg, host->host_addr);
	while (g_sig == 0)
	{
		icmp.un.echo.sequence = seq++;
		icmp.checksum = 0;
		icmp.checksum = checksum(&icmp, sizeof(icmp));

		// printf("send : type = %d code = %d, checksum = %d\n", icmp.type, icmp.code, icmp.checksum);
		gettimeofday(&packet_start, NULL);
		ssize_t sto = sendto(sockfd, &icmp, sizeof(icmp), 0, (const struct sockaddr *)host_addr, sizeof(*host_addr));
		if (sto == -1)
		{
			perror("sendto");
			return (0);
		}
		struct sockaddr_in	reply;
		socklen_t			rlen = sizeof(reply);
		ssize_t rfm = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&reply, &rlen);
		if (rfm == -1)
		{
			perror("recvfrom");
			return (0);
		}
		gettimeofday(&packet_end, NULL);

		printf("size sended = %lu\nsize received = %lu\n", sto, rfm);
		struct iphdr	*replyIp = (struct iphdr *)buff;
		struct icmphdr	*replyIcmp = (struct icmphdr *)(buff + sizeof(struct iphdr));
		// printf("reply icmp : type = %d code = %d, checksum = %d\n", replyIcmp->type, replyIcmp->code, replyIcmp->checksum);
		if (replyIcmp->type != 0)
			progval->pack_lost++;
		float packet_time = (packet_end.tv_sec - packet_start.tv_sec) + ((packet_end.tv_usec - packet_start.tv_usec) / 1000.0);
		printf("%s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n", host->domain_name, host->host_addr, seq, replyIp->ttl, packet_time);
		progval->ptime_total += packet_time;
		progval->ptime_max = (packet_time > progval->ptime_max) ? packet_time : progval->ptime_max;
		progval->ptime_min = (packet_time < progval->ptime_max || progval->ptime_min == 0) ? packet_time : progval->ptime_min;
		sleep(1);
	}
	progval->pack_total = seq;
	progval->ptime_avg = progval->ptime_total / seq;
	return (1);
}

void	print_result(packetvalue *progval, char *hostname)
{
	int rec_packet = progval->pack_total - progval->pack_lost;
	int loss_packet = (progval->pack_lost / progval->pack_total) * 100;
	printf("\n--- %s ping statistics ---\n%d packets transmitted, %d received, %d%% packet loss, time %dms\nrtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			hostname, progval->pack_total, rec_packet, loss_packet, (int)progval->total_time, progval->ptime_min, progval->ptime_avg, progval->ptime_max, progval->ptime_mdev);
	return ;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("ARG ERROR\n");
		exit(1);
	}
	signal_handler();

	packetvalue			progval;
	dnsinfo				host;
	struct sockaddr_in	hostaddr;
	memset(&progval, 0, sizeof(progval));
	memset(&host, 0, sizeof(host));
	memset(&hostaddr, 0, sizeof(hostaddr));

	if (!resolve_dns(&hostaddr, argv[1], &host))
		fprintf(stderr, "Dns resolver error\n");

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		fprintf(stderr, "invalid sockfd\n");
		return (1);
	}
	if (!ping_loop(sockfd, argv[1], &hostaddr, &progval, &host))
		return (printf("not good\n"), 1);
	print_result(&progval, argv[1]);
	free(host.domain_name);
	return (0);
}