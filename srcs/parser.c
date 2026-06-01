#include "ping.h"

static void	payload_size_check(char *str, ping_option_value *ping_opt)
{
	unsigned long long size = atol(str);
	if (size > 65399)
	{
		fprintf(stderr, "ft_ping: option value too big: %s\n", str);
		exit(1);
	}
	ping_opt->payload_size = (int)size;
	return ;
}

static void	time_ping_check(char *str, ping_option_value *ping_opt)
{
	unsigned long long max_time = atoll(str);
	if (max_time > 2147483647)
	{
		fprintf(stderr, "ft_ping: option value too big: %s\n", str);
		// fprintf(stderr, "ft_ping: invalid argument: %s: out of range: 0 <= value <= 9223372036854775807\n", str);
		exit(1);
	}
	else if (max_time == 0)
	{
		fprintf(stderr, "./ping/ping: option value too small: %s\n", str);
		exit(1);
	}
	ping_opt->max_possible_time = max_time;
}

static void	nbr_packet_check(char *str, ping_option_value *ping_opt)
{
	unsigned long long nbr_packet = atoll(str);
	// if (nbr_packet < 1 || nbr_packet > 9223372036854775807)
	// {
	// 	fprintf(stderr, "ft_ping: invalid argument: %s: out of range: 0 <= value <= 9223372036854775807\n", str);
	// 	exit(1);
	// }
	ping_opt->nbr_max_packet = nbr_packet;
	return ;
}

static void	preload_check(char *str, ping_option_value *ping_opt)
{
	unsigned long long preload = atoll(str);
	if (preload > 2147483647)
	{
		fprintf(stderr, "ft_ping: invalid preload value (%s)\n", str);
		// fprintf(stderr, "ft_ping: invalid argument: %s: out of range: 0 <= value <= 9223372036854775807\n", str);
		exit(1);
	}
	ping_opt->preload = preload;
}

static void	timeout_check(char *str, ping_option_value *ping_opt)
{
	unsigned long long sec = atoll(str);
	if (sec > 2147483647)
	{
		fprintf(stderr, "ft_ping: option value too big: %s\n", str);
		exit(1);
	}
	else if (sec == 0)
	{
		fprintf(stderr, "./ping/ping: option value too small: %s\n", str);
		exit(1);
	}
	ping_opt->timeout.tv_sec = sec;
}

static void	parse_int_option(char *str, size_t str_idx, char **strtab, size_t tabsize, ping_option_value *ping_opt, int *skip_arg, void (*check)(char *, ping_option_value *))
{
	int i = 0;

	int count = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]) && str[i] != '-' && str[i] != '+')
		{
			printf("ft_ping: invalid value: (`%1$ss\' near `%1$s\')\n", str);
			exit(1);
		}
		count++;
		i++;
	}
	if (count != 0) {
		(*check)(str, ping_opt);
		*skip_arg = 1;
		return ;
	}
	
	if (str_idx + 1 == tabsize)
		exit(1);
	char *next_str = strtab[str_idx + 1];
	i = 0;
	while (next_str[i])
	{
		if (!ft_isdigit(next_str[i]) && next_str[i] != '-' && next_str[i] != '+')
		{
			printf("ft_ping: invalid value: (`%1$s\' near `%1$s\')\n", next_str);
			exit(1);
		}
		count++;
		i++;
	}
	if (count != 0) {
		(*check)(next_str, ping_opt);
		*skip_arg = 1;
		return ;
	}
}

int		ft_ishex(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
		return (1);
	return (0);
}

void	payload_patern(char *str, size_t str_idx, char **strtab, size_t tabsize, ping_option_value *ping_opt, int *skip_arg)
{
	int i = 0;

	int count = 0;
	while (str[i])
	{
		if (!ft_ishex(str[i]) && i < 32)
		{
			printf("ft_ping: patterns must be specified as hex digits: %c\n", str[i]);
			exit(1);
		}
		count++;
		i++;
	}
	if (count != 0) {
		ping_opt->payload_patern = str;
		*skip_arg = 1;
		return ;
	}
	
	if (str_idx + 1 == tabsize)
		exit(1);
	char *next_str = strtab[str_idx + 1];
	i = 0;
	while (next_str[i])
	{
		if (!ft_ishex(next_str[i]) && i < 32)
		{
			printf("ft_ping: patterns must be specified as hex digits: %c\n", next_str[i]);
			exit(1);
		}
		count++;
		i++;
	}
	if (count != 0) {
		ping_opt->payload_patern = next_str;
		*skip_arg = 1;
		return ;
	}
}

int	parse_args(char **strtab, size_t tabsize, char **dsts, ping_flags *flags, ping_option_value *ping_opt)
{
	int skip_arg = 0;
	int dst_idx = 0;
	for (size_t i = 1; i < tabsize; i++)
	{
		if (skip_arg)
		{
			skip_arg = 0;
			continue;
		}
		if (*strtab[i] == '-')
		{
			size_t idx = 1;
			while (*(strtab[i] + idx))
			{
				if (skip_arg)
				{
					skip_arg = 0;
					break;
				}
				switch (*(strtab[i] + idx))
				{
					case 'v': flags->v_flag = 1; //mandatory
						break;
					case '?': flags->qm_flag = 1; //madatory
						break;
					case 'n': flags->n_flag = 1; //a revoir
						break;
					case 'q': flags->q_flag = 1;
						break;
					case 'f': flags->f_flag = 1;
						break;
					case 's': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, ping_opt, &skip_arg, payload_size_check);
						break;
					case 'c': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, ping_opt, &skip_arg, nbr_packet_check);
						break;
					case 'w': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, ping_opt, &skip_arg, time_ping_check);
						break;
					case 'W': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, ping_opt, &skip_arg, timeout_check);
						break;
					case 'l': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, ping_opt, &skip_arg, preload_check);
						break;
					case 'p': payload_patern(strtab[i] + idx + 1, i, strtab, tabsize, ping_opt, &skip_arg);
						break;
					case '\0' : return (-1);
						break;
					default: return (*(strtab[i] + idx));
						break;
				}
				idx++;
			}
		}
		else
		{
			dsts[dst_idx++] = strtab[i];
		}
	}
	if (flags->qm_flag)
	{
		printf("%s\n", help());
		exit(2);
	}
	if (dst_idx == 0)
	{
		fprintf(stderr, "ft_ping: missing host operand\n%s\n", wrong_usage());
		exit(1);
	}
	dsts[dst_idx] = NULL;
	return (0);
}