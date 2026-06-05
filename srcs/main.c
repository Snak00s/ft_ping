#include"ping.h"

int	g_sig = 0;

static void	handler(int sig)
{
	(void)sig;
	g_sig = 1;
}

static void	signal_handler(void)
{
	struct sigaction	sa;
	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = &handler;
	sigaction(SIGINT, &sa, 0);
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

	char *destinations[argc];

	int ret = parse_args(argv, argc, destinations, &flags, &ping_opt);
	if (ret > 0)
		return (fprintf(stderr, "ft_ping: invalid option -- \'%c\'\n%s\n", (char)ret, wrong_usage()), 2);
	else if (ret == -1)
		return (fprintf(stderr, "ft_ping: : Name or service not known\n"), 2);

	int nbr_dest = 0;
	while (destinations[nbr_dest])
		nbr_dest++;

	struct addrinfo		hints = {
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_RAW,
		.ai_flags = AI_CANONNAME
	};
	dns_info			host[nbr_dest];
	struct sockaddr_in	hostaddr[nbr_dest];
	for (int i = 0; i != nbr_dest; i++)
	{
		host[i].cannon_name = NULL;
		host[i].domain_name = NULL;
		if (!resolve_dns(destinations[i], &hostaddr[i], &host[i], &hints))
			return (2);
		host[i].argv_dest = destinations[i];
		host[i].ping_started = 0;
	}

	int sockfd = socket(hostaddr[0].sin_family, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		if (errno == EPERM)
			fprintf(stderr, "ft_ping: Lacking privilege for icmp socket.\n");
		else
			fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dns_info(host, nbr_dest);
		return (1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &ping_opt.timeout, sizeof(ping_opt.timeout)) < 0)
	{
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dns_info(host, nbr_dest);
		close(sockfd);
	}

	signal_handler();
	ret = ping_loop(sockfd, hostaddr, host, nbr_dest, &ping_opt, &flags, (flags.f_flag == 1 || flags.l_flag == 1) ? MSG_DONTWAIT : 0);
	free_dns_info(host, nbr_dest);
	close(sockfd);
	return (ret);
}