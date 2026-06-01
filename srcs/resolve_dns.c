#include"ping.h"

int resolve_dns(char *domain_name, struct sockaddr_in *hostaddr, dns_info *host, struct addrinfo *hints)
{
	struct addrinfo *result;

	int gai_rc = getaddrinfo(domain_name, NULL, hints, &result);
	switch (gai_rc)
	{
		case 0:
			break;
		case EAI_SYSTEM:
			fprintf(stderr, "ft_ping: %s: system error %s\n", domain_name, strerror(errno));
			return (0);
		case EAI_AGAIN:
			fprintf(stderr, "ft_ping: %s: unknown host\n", domain_name);
			return (0);
		default:
			fprintf(stderr, "ft_ping: %s: %s\n", domain_name, gai_strerror(gai_rc));
			return (0);
	}

	struct addrinfo	*ai = result;
	while (ai != NULL)
	{
		char name[1025];
		char addr[1025];
		int gni_rc = getnameinfo(ai->ai_addr, ai->ai_addrlen, addr, sizeof(addr), NULL, 0, NI_NUMERICHOST);
		if (gni_rc != 0) {
			fprintf(stderr, "ft_ping: %s\n", gai_strerror(gni_rc));
			continue;
		}
		gni_rc = getnameinfo(ai->ai_addr, ai->ai_addrlen, name, sizeof(name), NULL, 0, 0);
		switch (gni_rc)
		{
			case 0:
				break;
			case EAI_SYSTEM:
				fprintf(stderr, "ft_ping: %s: system error %s\n", addr, strerror(errno));
				break;
			default:
				fprintf(stderr, "ft_ping: %s: %s\n", addr, gai_strerror(gni_rc));
		}

		if (parse_ip(addr))
		{
			ft_strlcpy(host->host_addr, addr, ft_strlen(addr) + 1);
			host->domain_name = ft_strdup(name);
			if (!host->domain_name)
				return (0);
			if (ai->ai_canonname)
			{
				host->cannon_name = ft_strdup(ai->ai_canonname);
				if (!host->cannon_name)
					return (0);
			}
			hostaddr->sin_family = ai->ai_family;
			break;
		}
		ai = ai->ai_next;
	}
	freeaddrinfo(result);
	if (!host->domain_name)
		return (0);
	hostaddr->sin_port = htons(80);
	inet_pton(hostaddr->sin_family, host->host_addr, &hostaddr->sin_addr);
	return (1);
}
