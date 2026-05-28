#include"ping.h"

int	g_sig = 0;
int g_alrm = 1;

void	handler(int sig)
{
	(void)sig;
	g_sig = 1;
}

void	alarm_handler(int sig)
{
	(void)sig;
	g_alrm = 1;
}

void	signal_handler(void)
{
	struct sigaction	sa;
	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = &handler;
	sigaction(SIGINT, &sa, 0);

	struct sigaction	alrm_sa;
	ft_memset(&alrm_sa, 0, sizeof(struct sigaction));
	alrm_sa.sa_handler = &alarm_handler;
	sigaction(SIGALRM, &alrm_sa, 0);
}

int	print_result(packetvalue *progval, char *hostname)
{
	int rec_packet = progval->pack_total - progval->pack_lost;
	int percent_loss = (progval->pack_lost * 100) / progval->pack_total;
	printf("--- %s ping statistics ---\n%d packets transmitted, %d packets received, %d%% packet loss\n",
			hostname, progval->pack_total, rec_packet, percent_loss);
	if (percent_loss != 100)
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", progval->ptime_min, progval->ptime_avg, progval->ptime_max, progval->ptime_mdev);
	return (percent_loss == 100);
}

char	*help()
{
	return ("Usage\n\
  ping [options] <destination>\n\
\n\
Options:\n\
  <destination>      dns name or ip address\n\
  -f                 flood ping\n\
  -l <preload>       send <preload> number of packages while waiting replies\n\
  -n                 no dns name resolution\n\
  -p <pattern>       contents of padding byte\n\
  -q                 quiet output\n\
  -s <size>          use <size> as number of data bytes to be sent\n\
  -T <timestamp>     define timestamp, can be one of <tsonly|tsandaddr|tsprespec>\n\
  -v                 verbose output\n\
  -w <deadline>      reply wait <deadline> in seconds\n\
  -W <timeout>       time to wait for response\n");
}

int main(int argc, char **argv)
{
	ping_flags			flags;
	packetvalue			progval;
	dnsinfo				host;
	struct sockaddr_in	hostaddr;
	struct addrinfo		hints = {
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_RAW,
		.ai_flags = AI_CANONNAME
	};

	ft_memset(&flags, 0, sizeof(flags));
	ft_memset(&progval, 0, sizeof(progval));
	ft_memset(&host, 0, sizeof(host));
	ft_memset(&hostaddr, 0, sizeof(hostaddr));
	progval.payload_size = 56;

	int	dst = 0;

	int ret = parse_args(argv, argc, &dst, &flags, &progval);
	if (ret > 0)
		return (fprintf(stderr, "ft_ping: invalid option -- \'%c\'\n\n%s", (char)ret, help()), 2);
	else if (ret == -1)
		return (fprintf(stderr, "ft_ping: : Name or service not known\n"), 2);

	if (!resolve_dns(argv[dst], &hostaddr, &host, &hints))
		return (2);

	int sockfd = socket(hostaddr.sin_family, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dnsinfo(&host);
		return (1);
	}
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dnsinfo(&host);
		close(sockfd);
	}

	signal_handler();
	host.argv_dest = argv[dst];
	if (!ping_loop(sockfd, &hostaddr, &progval, &host, &flags))
	{
		free_dnsinfo(&host);
		close(sockfd);
		return (1);
	}
	ret = print_result(&progval, argv[dst]);
	free_dnsinfo(&host);
	close(sockfd);
	return (ret);
}