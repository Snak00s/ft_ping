#ifndef PING_H
# define PING_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/in_systm.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <string.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/time.h>
# include <signal.h>
# include <errno.h>
# include "../libft/libft.h"

typedef struct packetvalue {
	float			ptime_min;
	float			ptime_avg;
	float			ptime_max;
	float			ptime_mdev;
	float			total_time;
	float			ptime_total;
	unsigned int	pack_lost;
	unsigned int	pack_total;
} packetvalue;

typedef struct dnsinfo {
	char	host_addr[20];
	char	*domain_name;
} dnsinfo;

typedef struct packet {
	struct	icmphdr	icmp;
	struct	iphdr	ip;
	char			garbage[56];
} packet;

int resolve_dns(struct sockaddr_in *host_addr, char *domain_name, dnsinfo *host);

#endif