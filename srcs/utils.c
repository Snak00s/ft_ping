#include"ping.h"

char	*help()
{
	return ("Usage: ping [OPTION...] HOST ...\n\
Send ICMP ECHO_REQUEST packets to network hosts.\n\
\n\
 Options valid for all request types:\n\
\n\
  -c                         stop after sending NUMBER packets\n\
  -n                         do not resolve host addresses\n\
  -v                         verbose output\n\
  -w                         stop after N seconds\n\
  -W                         number of seconds to wait for response\n\
\n\
 Options valid for --echo requests:\n\
\n\
  -f                         flood ping (root only)\n\
  -l                         send NUMBER packets as fast as possible before\n\
                             falling into normal mode of behavior (root only)\n\
  -p                         fill ICMP packet with given pattern (hex)\n\
  -q                         quiet output\n\
  -s                         send NUMBER data octets\n\
\n\
  -?                         give this help list");
}

char	*wrong_usage()
{
	return ("Try 'ping -?' for more information.");
}

// void	free_dns_info(dns_info *dns, int nbr_dest)
// {
// 	for (int i = 0; i != nbr_dest; i++)
// 	{
// 		free(dns[i].cannon_name);
// 		dns[i].cannon_name = NULL;
// 		free(dns[i].domain_name);
// 		dns[i].domain_name = NULL;
// 	}
// }

int	parse_ip(char *addr)
{
	if (ft_strlen(addr) > 16)
		return (0);
	
	int i = 0;
	while (addr[i])
	{
		if (!isdigit(addr[i]) && addr[i] != '.')
			return (0);
		i++;
	}
	return (1);
}