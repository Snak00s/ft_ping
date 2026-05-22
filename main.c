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
	int loss_packet = (progval->pack_lost / progval->pack_total) * 100;
	printf("\n--- %s ping statistics ---\n%d packets transmitted, %d received, %d%% icmp_packet loss, time %dms\n",
			hostname, progval->pack_total, rec_packet, loss_packet, (int)progval->total_time);
	if (loss_packet != 100)
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", progval->ptime_min, progval->ptime_avg, progval->ptime_max, progval->ptime_mdev);
	return (loss_packet == 100);
}

int	parse_args(char **strtab, size_t tabsize, ping_flags *flags)
{
	int dest_amount = 0;
	for (size_t i = 1; i < tabsize; i++)
	{
		if (*strtab[i] == '-')
		{
			switch (*(strtab[i] + 1))
			{
				case 'v': flags->v_flag = 1;
					break;
				case '?': flags->qm_flag = 1;
					break;
				case 'n': flags->n_flag = 1;
					break;
				case '\0' : return (-1);
					break;
				default: return (*(strtab[i] + 1));
					break;
			}
		}
		else
			dest_amount++;
	}
	if (dest_amount == 0)
	{
		fprintf(stderr, "ping: usage error: Destination address required\n");
		exit(1);
	}
	return (0);
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
	dnsinfo				host, l_host;
	struct sockaddr_in	hostaddr, l_hostaddr;
	struct addrinfo		hints = {
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_RAW,
		.ai_flags = AI_CANONNAME
	};
	ft_memset(&flags, 0, sizeof(flags));
	ft_memset(&progval, 0, sizeof(progval));
	ft_memset(&host, 0, sizeof(host));
	ft_memset(&l_host, 0, sizeof(l_host));
	ft_memset(&l_hostaddr, 0, sizeof(l_hostaddr));
	ft_memset(&hostaddr, 0, sizeof(hostaddr));

	int ret = parse_args(argv, argc, &flags);
	if (ret > 0)
		return (fprintf(stderr, "ft_ping: invalid option -- \'%c\'\n\n%s", (char)ret, help()), 1);
	else if (ret == -1)
		return (fprintf(stderr, "ft_ping: : Name or service not known\n"), 2);
	if (flags.qm_flag)
		return (printf("%s\n", help()), 1);

	int last_dom = 0;
	int	fisrt_dom = 0;
	for (int i = 1; i < argc; i++)
	{
		if (*argv[i] != '-')
		{
			if (!fisrt_dom)
			{
				if (!resolve_dns(argv[i], &hostaddr, &host, &hints))
					return (1);
				fisrt_dom = i;
			}
			else
			{
				if (!resolve_dns(argv[i], &l_hostaddr, &l_host, &hints))
				{
					free_dnsinfo(&host);
					free_dnsinfo(&l_host);
					return (1);
				}
			}
			last_dom = i;
		}
	}
	if (fisrt_dom == last_dom && fisrt_dom != 0)
	{
		l_hostaddr = hostaddr;
		l_host.cannon_name = ft_strdup(host.cannon_name);
		l_host.domain_name = ft_strdup(host.domain_name);
		ft_strlcpy(l_host.host_addr, host.host_addr, ft_strlen(host.host_addr) + 1);
	}

	int sockfd = socket(hostaddr.sin_family, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dnsinfo(&host);
		free_dnsinfo(&l_host);
		return (1);
	}
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		free_dnsinfo(&host);
		free_dnsinfo(&l_host);
		close(sockfd);
	}

	if (flags.v_flag)
		printf("ping: sock4.fd: %d (socktype: %s), hints.ai_family: %s\n\nai->ai_family: %s, ai->ai_canonname: \'%s\'\n",
			sockfd, sock_name(SOCK_RAW), af_name(hints.ai_family), af_name(l_hostaddr.sin_family), l_host.cannon_name);

	signal_handler();
	if (!ping_loop(sockfd, argv[last_dom], &hostaddr, &progval, &host, &l_host, &flags))
	{
		free_dnsinfo(&host);
		free_dnsinfo(&l_host);
		close(sockfd);
		printf("not good\n");
		return (1);
	}
	ret = print_result(&progval, argv[last_dom]);

	free_dnsinfo(&host);
	free_dnsinfo(&l_host);
	close(sockfd);
	return (ret);
}