#include"ping.h"

int	g_sig = 0;
int g_alrm = 1;

static void	handler(int sig)
{
	(void)sig;
	g_sig = 1;
}

static void	alarm_handler(int sig)
{
	(void)sig;
	g_alrm = 1;
}

static void	signal_handler(void)
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
	ft_memset(&flags, 0, sizeof(flags));

	ping_option_value	ping_opt;
	ft_memset(&ping_opt, 0, sizeof(ping_opt));
	ping_opt.payload_size = 56;
	ping_opt.timeout.tv_sec = 10;
	ping_opt.timeout.tv_usec = 0;

	int	dst = 0;
	// char *destinations[argc];

	int ret = parse_args(argv, argc, &dst,/* destinations,*/ &flags, &ping_opt);
	if (ret > 0)
		return (fprintf(stderr, "ft_ping: invalid option -- \'%c\'\n\n%s", (char)ret, help()), 2);
	else if (ret == -1)
		return (fprintf(stderr, "ft_ping: : Name or service not known\n"), 2);

	// int nbr_dest = 0;
	// while (destinations[nbr_dest])
	// 	nbr_dest++;

	dns_info			host;
	ft_memset(&host, 0, sizeof(host));

	struct sockaddr_in	hostaddr;
	ft_memset(&hostaddr, 0, sizeof(hostaddr));

	struct addrinfo		hints = {
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_RAW,
		.ai_flags = AI_CANONNAME
	};

	if (!resolve_dns(argv[dst], &hostaddr, &host, &hints))
		return (2);

	int sockfd = socket(hostaddr.sin_family, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dns_info(&host);
		return (1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &ping_opt.timeout, sizeof(ping_opt.timeout)) < 0)
	{
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dns_info(&host);
		close(sockfd);
	}

	signal_handler();
	host.argv_dest = argv[dst];
	ret = ping_loop(sockfd, &hostaddr, &ping_opt, &host, &flags);
	free_dns_info(&host);
	close(sockfd);
	return (ret);
}