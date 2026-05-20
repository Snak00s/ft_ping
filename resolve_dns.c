#include"ping.h"

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

int resolve_dns(struct sockaddr_in *host_addr, char *domain_name, dnsinfo *host, struct addrinfo *hints)
{
	struct addrinfo *result;

	int gai_rc = getaddrinfo(domain_name, NULL, hints, &result);
	switch (gai_rc) {
		case 0:
			break;
		case EAI_SYSTEM:
			fprintf(stderr, "ft_ping: %s: system error %s\n", domain_name, strerror(errno));
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
		switch (gni_rc) {
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
			free(host->domain_name);
			free(host->cannon_name);
			host->domain_name = ft_strdup(name);
			if (!host->domain_name)
				return (0);
			host->cannon_name = ft_strdup(ai->ai_canonname);
			if (!host->cannon_name)
				return (0);
			host_addr->sin_family = ai->ai_family;
			break;
		}
		ai = ai->ai_next;
	}
	freeaddrinfo(result);

	if (!host->domain_name)
		return (0);
	host_addr->sin_port = htons(80);
	inet_pton(host_addr->sin_family, host->host_addr, &host_addr->sin_addr);

	return (1);
}
