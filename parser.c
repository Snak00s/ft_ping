#include "ping.h"

void	payload_size_check(char *str, packetvalue *progval)
{
	long size = atol(str);
	if (size < 0 || size > 2147483647)
	{
		fprintf(stderr, "ft_ping: invalid argument: %s: out of range: 0 <= value <= 2147483647\n", str);
		exit(1);
	}
	progval->payload_size = (int)size;
	return ;
}

void	nbr_packet_check(char *str, packetvalue *progval)
{
	unsigned long long nbr_packet = atoll(str);
	if (nbr_packet < 1 || nbr_packet > 9223372036854775807)
	{
		fprintf(stderr, "ft_ping: invalid argument: %s: out of range: 0 <= value <= 9223372036854775807\n", str);
		exit(1);
	}
	progval->nbr_max_packet = nbr_packet;
	return ;
}

void	parse_int_option(char *str, size_t str_idx, char **strtab, size_t tabsize, packetvalue *progval, int *skip_arg, void (*check)(char *, packetvalue *))
{
	int i = 0;

	int count = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]) && str[i] != '-' && str[i] != '+')
		{
			printf("ft_ping: invalid argument: \'%s\'\n", str);
			exit(1);
		}
		count++;
		i++;
	}
	if (count != 0) {
		(*check)(str, progval);
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
			printf("ft_ping: invalid argument: \'%s\'\n", next_str);
			exit(1);
		}
		count++;
		i++;
	}
	if (count != 0) {
		(*check)(next_str, progval);
		*skip_arg = 1;
		return ;
	}
}

int	parse_args(char **strtab, size_t tabsize, int *destination, ping_flags *flags, packetvalue *progval)
{
	int skip_arg = 0;
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
					case 'v': flags->v_flag = 1;
						break;
					case '?' || 'h': flags->qm_flag = 1;
						break;
					case 'n': flags->n_flag = 1;
						break;
					case 'D': flags->D_flag = 1;
						break;
					case 's': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, progval, &skip_arg, payload_size_check);
						break;
					case 'c': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, progval, &skip_arg, nbr_packet_check);
						break;
					case 'W': parse_int_option(strtab[i] + idx + 1, i, strtab, tabsize, progval, &skip_arg, nbr_packet_check);
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
			if (*destination != 0) {
				fprintf(stderr, "ft_ping: usage error: Can accept only 1 destination\n");
				exit(1);
			}
			*destination = i;
		}
	}
	if (flags->qm_flag)
	{
		printf("%s\n", help());
		exit(2);
	}
	if (*destination == 0)
	{
		fprintf(stderr, "ft_ping: usage error: Destination address required\n");
		exit(1);
	}
	return (0);
}