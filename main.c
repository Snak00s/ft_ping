#include"ping.h"

int	g_sig = 0;

void	handler(int sig)
{
	(void)sig;
	g_sig = 1;
}

void	signal_handler(void)
{
	struct sigaction	sa;
	ft_memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = &handler;
	sigaction(SIGINT, &sa, 0);
}

void	print_result(packetvalue *progval, char *hostname)
{
	int rec_packet = progval->pack_total - progval->pack_lost;
	int loss_packet = (progval->pack_lost / progval->pack_total) * 100;
	printf("\n--- %s ping statistics ---\n%d packets transmitted, %d received, %d%% icmp_packet loss, time %dms\nrtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			hostname, progval->pack_total, rec_packet, loss_packet, (int)progval->total_time, progval->ptime_min, progval->ptime_avg, progval->ptime_max, progval->ptime_mdev);
	return ;
}

int	parse_args(char **strtab, size_t tabsize, ping_flags *flags)
{
	for (size_t i = 0; i < tabsize; i++)
	{
		if (*strtab[i] == '-')
		{
			switch (*(strtab[i] + 1))
			{
			case 'v': flags->v_flag = 1;
				break;
			case '?': flags->qm_flag = 1;
				break;
			default: return (*(strtab[i] + 1));
				break;
			}
		}
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
	signal_handler();

	ping_flags			flags;
	memset(&flags, 0, sizeof(flags));

	if (parse_args(argv, argc, &flags))
		return (1);

	if (flags.qm_flag)
	{
		printf("%s\n", help());
		return (1);
	}

	packetvalue			progval;
	dnsinfo				host;
	struct sockaddr_in	hostaddr;
	struct addrinfo		hints = {
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_RAW,
		.ai_flags = AI_CANONNAME
	};
	memset(&progval, 0, sizeof(progval));
	memset(&host, 0, sizeof(host));
	memset(&hostaddr, 0, sizeof(hostaddr));

	for (int i = 0; i < argc; i++)
	{
		if (*argv[i] != '-')
		{
			if (!resolve_dns(&hostaddr, argv[1], &host, &hints))
				return (1);
		}
	}

	int sockfd = socket(hostaddr.sin_family, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		fprintf(stderr, "ft_ping: %s\n", strerror(errno));
		return (1);
	}

	if (flags.v_flag)
		printf("ping: sock4.fd: %d (socktype: %s), hints.ai_family: %s\n\nai->ai_family: %s, ai->ai_canonname: \'%s\'\n",
			sockfd, sock_name(SOCK_RAW), af_name(hints.ai_family), af_name(hostaddr.sin_family), host.cannon_name);

	if (!ping_loop(sockfd, argv[1], &hostaddr, &progval, &host, &flags))
	{
		close(sockfd);
		printf("not good\n");
		return (1);
	}
	print_result(&progval, argv[1]);
	free(host.domain_name);
	free(host.cannon_name);
	close(sockfd);
	return (0);
}