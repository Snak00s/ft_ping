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

int ping_loop(int sockfd, struct sockaddr_in *host_addr, endprogvalue *progval)
{
	struct icmphdr	icmp;
	struct timeval	packet_start, packet_end;
	int				seq = 0;
	char buff[1024];

	icmp.type = ICMP_ECHO;			//ne bouge pas a chaque boucle
	icmp.code = 0;					//ne bouge pas a chaque boucle
	icmp.un.echo.id = getpid();		//ne bouge pas a chaque boucle

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

		struct iphdr	*replyIp = (struct iphdr *)buff;
		struct icmphdr	*replyIcmp = (struct icmphdr *)(buff + sizeof(struct iphdr));
		// printf("reply ip : ttl = %d\n", replyIp->ttl);
		// printf("reply icmp : type = %d code = %d, checksum = %d\n", replyIcmp->type, replyIcmp->code, replyIcmp->checksum);
		if (replyIcmp->type != 0)
			progval->pack_lost++;
		float packet_time = (packet_end.tv_sec - packet_start.tv_sec) + ((packet_end.tv_usec - packet_start.tv_usec)/1000.0);
		printf(": icmp_seq=%d ttl=%d time=%.2f ms\n", seq, replyIp->ttl, packet_time);
		progval->ptime_total += packet_time;
		progval->ptime_max = packet_time > progval->ptime_max ? packet_time : progval->ptime_max;
		progval->ptime_min = packet_time < progval->ptime_max || progval->ptime_min == 0 ? packet_time : progval->ptime_min;
		sleep(1);
	}

	return (1);
}

int resolve_dns(struct sockaddr_in *host_addr, char *domain_name, char host_ip[16])
{
	struct hostent	*host_info = gethostbyname(domain_name);
	if (!host_info || !(struct in_addr *)(host_info->h_addr))
		return (0);
	printf("before completion %u\n", host_addr->sin_addr.s_addr);
	host_addr->sin_family = AF_INET;
	host_addr->sin_port = htons(80);
	char *ip = inet_ntoa(*(struct in_addr *)(host_info->h_addr));
	inet_pton(AF_INET, ip, &host_addr->sin_addr);

	ft_strlcpy(host_ip, ip, ft_strlen(ip));
	return (1);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("ARG ERROR\n");
		exit(1);
	}
	signal_handler();

	endprogvalue	progval;
	struct sockaddr_in hostaddr;
	memset(&progval, 0, sizeof(progval));
	memset(&hostaddr, 0, sizeof(hostaddr));

	if (!resolve_dns(&hostaddr, argv[1], progval.host_ip))
		printf("Dns resilver error\n");

	printf("after completion %u\n", hostaddr.sin_addr.s_addr);

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		printf("invalid sockfd\n");
		return (1);
	}
	if (!ping_loop(sockfd, &hostaddr, &progval))
		return (printf("not good\n"), 1);
	printf("good i guess\n");
	return (0);
}