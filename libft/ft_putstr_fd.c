/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:01:42 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/14 17:15:26 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}
/*
#include<fcntl.h>
int	main(int argc, char **argv)
{
	char	str[] = "feur";
	int	fd;

	fd = open(argv[1], O_WRONLY);
	ft_putstr_fd(str, fd);
	close(fd);
	return (0);
}*/
