#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include "libft/libft.h"

char	buffer[1000];

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

int ping_loop(int sockfd, struct sockaddr_in *host_addr)
{
	struct icmphdr	icmp;
	// struct iphdr	ip;
	// struct timeval	tv;
	int				seq = 0;
	char buff[128];

	icmp.type = ICMP_ECHO;			//ne bouge pas a chaque boucle
	icmp.code = 0;					//ne bouge pas a chaque boucle
	icmp.un.echo.id = getpid();		//ne bouge pas a chaque boucle
	icmp.un.echo.sequence = seq++;
	icmp.checksum = 0;
	icmp.checksum = checksum(&icmp, sizeof(icmp));

	ssize_t sto = sendto(sockfd, &icmp, sizeof(icmp), 0, (const struct sockaddr *)host_addr, sizeof(host_addr));
	if (sto == -1)
	{
		printf("sendto error\n");
		return (0);
	}
	printf("sended: %lu\n", sto);
	ssize_t rfm = recvfrom(sockfd, &buff, sizeof(buff), 0, (const struct sockaddr *)host_addr, sizeof(host_addr));
	if (rfm == -1)
	{
		printf("recvfrom error\n");
		return (0);
	}
	printf("recevived: %lu\n", rfm);
	return (0);
}

int resolve_dns(struct sockaddr_in *host_addr, char *domain_name)
{
	struct hostent	*host_info = gethostbyname(domain_name);
	if (!host_info || !(struct in_addr *)(host_info->h_addr))
		return (0);
	printf("before completion %u\n", host_addr->sin_addr.s_addr);
	host_addr->sin_addr.s_addr = *(long *) (host_info->h_addr);
	host_addr->sin_family = AF_INET;
	host_addr->sin_port = htons(80);
	// inet_pton(AF_INET, inet_ntoa(*(struct in_addr *) (host_info->h_addr)), &servaddr.sin_addr);
	return (1);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("ARG ERROR\n");
		exit(1);
	}
	struct sockaddr_in hostaddr;
	memset(&hostaddr, 0, sizeof(hostaddr));

	if (!resolve_dns(&hostaddr, argv[1]))
		printf("Dns resilver error\n");

	printf("after completion %u\n", hostaddr.sin_addr.s_addr);

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		printf("invalid sockfd\n");
		return (1);
	}
	ping_loop(sockfd, &hostaddr);
	printf("good i guess\n");
	return (0);
}