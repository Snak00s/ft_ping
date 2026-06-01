#include"ping.h"

char *af_name(sa_family_t ai_family)
{
	switch (ai_family)
	{
	case AF_UNSPEC:
		return("AF_UNSPEC");
		break;
	case AF_LOCAL:
		return("AF_LOCAL");
		break;
	case AF_INET:
		return("AF_INET");
		break;
	case AF_AX25:
		return("AF_AX25");
		break;
	case AF_IPX:
		return("AF_IPX");
		break;
	case AF_APPLETALK:
		return("AF_APPLETALK");
		break;
	case AF_NETROM:
		return("AF_NETROM");
		break;
	case AF_BRIDGE:
		return("AF_BRIDGE");
		break;
	case AF_ATMPVC:
		return("AF_ATMPVC");
		break;
	case AF_X25:
		return("AF_X25");
		break;
	case AF_INET6:
		return("AF_INET6");
		break;
	case AF_ROSE:
		return("AF_ROSE");
		break;
	case AF_DECnet:
		return("AF_DECnet");
		break;
	case AF_NETBEUI:
		return("AF_NETBEUI");
		break;
	case AF_SECURITY:
		return("AF_SECURITY");
		break;
	case AF_KEY:
		return("AF_KEY");
		break;
	case AF_NETLINK:
		return("AF_NETLINK");
		break;
	case AF_PACKET:
		return("AF_PACKET");
		break;
	case AF_ASH:
		return("AF_ASH");
		break;
	case AF_ECONET:
		return("AF_ECONET");
		break;
	case AF_ATMSVC:
		return("AF_ATMSVC");
		break;
	case AF_RDS:
		return("AF_RDS");
		break;
	case AF_SNA:
		return("AF_SNA");
		break;
	case AF_IRDA:
		return("AF_IRDA");
		break;
	case AF_PPPOX:
		return("AF_PPPOX");
		break;
	case AF_WANPIPE:
		return("AF_WANPIPE");
		break;
	case AF_LLC:
		return("AF_LLC");
		break;
	case AF_IB:
		return(" AF_IB");
		break;
	case AF_MPLS:
		return("AF_MPLS");
		break;
	case AF_CAN:
		return("AF_CAN");
		break;
	case AF_TIPC:
		return("AF_TIPC");
		break;
	case AF_BLUETOOTH:
		return("AF_BLUETOOTH");
		break;
	case AF_IUCV:
		return("AF_IUCV");
		break;
	case AF_RXRPC:
		return("AF_RXRPC");
		break;
	case AF_ISDN:
		return("AF_ISDN");
		break;
	case AF_PHONET:
		return("AF_PHONET");
		break;
	case AF_IEEE802154:
		return("AF_IEEE802154");
		break;
	case AF_CAIF:
		return("AF_CAIF");
		break;
	case AF_ALG:
		return("AF_ALG");
		break;
	case AF_NFC:
		return("AF_NFC");
		break;
	case AF_VSOCK:
		return("AF_VSOCK");
		break;
	case AF_KCM:
		return("AF_KCM");
		break;
	case AF_QIPCRTR:
		return("AF_QIPCRTR");
		break;
	case AF_SMC:
		return("AF_SMC");
		break;
	case AF_XDP:
		return("AF_XDP");
		break;
	case AF_MCTP:
		return("AF_MCTP");
		break;
	case AF_MAX:
		return("AF_MAX");
		break;

	default:
		return(NULL);
		break;
	}
}


char *sock_name(int sock_type)
{
	switch (sock_type)
	{
	case SOCK_STREAM:
		return("SOCK_STREAM");
		break;
	case SOCK_DGRAM:
		return("SOCK_DGRAM");
		break;
	case SOCK_RAW:
		return("SOCK_RAW");
		break;
	case SOCK_RDM:
		return("SOCK_RDM");
		break;
	case SOCK_SEQPACKET:
		return("SOCK_SEQPACKET");
		break;
	case SOCK_DCCP:
		return("SOCK_DCCP");
		break;
	case SOCK_PACKET:
		return("SOCK_PACKET");
		break;
	case SOCK_CLOEXEC:
		return("SOCK_CLOEXEC");
		break;
	case SOCK_NONBLOCK:
		return("SOCK_NONBLOCK");
		break;
	
	default:
		return(NULL);
		break;
	}
}

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

void	free_dns_info(dns_info *dns, int nbr_dest)
{
	for (int i = 0; i != nbr_dest; i++)
	{
		free(dns[i].cannon_name);
		dns[i].cannon_name = NULL;
		free(dns[i].domain_name);
		dns[i].domain_name = NULL;
	}
}

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