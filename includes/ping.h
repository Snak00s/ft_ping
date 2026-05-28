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

typedef struct	packet_value {
	float			ptime_min;
	float			ptime_avg;
	float			ptime_max;
	float			ptime_mdev;
	float			total_time;
	float			ptime_total;
	float			ptime_total_2;
	unsigned int	pack_lost;
	unsigned int	pack_total;
} packet_value;

typedef struct	ping_option_value {
	struct timeval	timeout;
	long			payload_size;
	unsigned int	nbr_max_packet;
	unsigned int	max_possible_time;
	unsigned int	preload;
} ping_option_value;


typedef struct	dns_info {
	char	host_addr[20];
	char	*cannon_name;
	char	*domain_name;
	char	*argv_dest;
} dns_info;

typedef struct	icmp_packet {
	struct icmphdr	icmp;
	char			garbage[];
} icmp_packet;

typedef struct	ping_flags {
	int c_flag;
	int l_flag;
	int n_flag;
	int q_flag;
	int	qm_flag;
	int s_flag;
	int	v_flag;
} ping_flags;

//-----------------------------------------------------------

int		resolve_dns(char *domain_name, struct sockaddr_in *host_addr, dns_info *host, struct addrinfo *hints);
int		ping_loop(int sockfd, struct sockaddr_in *host_addr, ping_option_value *ping_opt, dns_info *host, ping_flags *flags);
char	*sock_name(int sock_type);
char	*af_name(sa_family_t ai_family);
char	*help();
void	free_dns_info(dns_info *dns);
int		parse_ip(char *addr);
int		parse_args(char **strtab, size_t tabsize, int *destination, ping_flags *flags, ping_option_value *ping_opt);

#endif