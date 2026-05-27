#ifndef PING_H
# define PING_H

# include <math.h>
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

extern int g_sig;
extern int g_alrm;

//-----------------------------------------------------------

typedef struct	packetvalue {
	float			ptime_min;
	float			ptime_avg;
	float			ptime_max;
	float			ptime_mdev;
	float			total_time;
	float			ptime_total;
	float			ptime_total_2;
	unsigned int	pack_lost;
	unsigned int	pack_total;
	long			payload_size;
	unsigned long long			nbr_max_packet;
} packetvalue;

typedef struct	dnsinfo {
	char	host_addr[20];
	char	*cannon_name;
	char	*domain_name;
} dnsinfo;

typedef struct	icmp_packet {
	struct icmphdr	icmp;
	char			garbage[];
} icmp_packet;

typedef struct	ping_flags {
	int	v_flag;
	int	qm_flag;
	int n_flag;
	int D_flag;
	int s_flag;
	int c_flag;
} ping_flags;

//-----------------------------------------------------------

int		resolve_dns(char *domain_name, struct sockaddr_in *host_addr, dnsinfo *host, struct addrinfo *hints);
int		ping_loop(int sockfd, char *arg, struct sockaddr_in *host_addr, packetvalue *progval, dnsinfo *host, ping_flags *flags);
char	*sock_name(int sock_type);
char	*af_name(sa_family_t ai_family);
char	*help();
void	free_dnsinfo(dnsinfo *dns);
int		parse_ip(char *addr);
int		parse_args(char **strtab, size_t tabsize, int *destination, ping_flags *flags, packetvalue *progval);

#endif