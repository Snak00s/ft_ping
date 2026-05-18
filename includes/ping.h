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
#include "../libft/libft.h"

typedef struct endprogvalue {
	float			ptime_min;
	float			ptime_avg;
	float			ptime_max;
	float			ptime_mdev;
	float			total_time;
	float			ptime_total;
	unsigned int	pack_lost;
	char			host_ip[16];
} endprogvalue;