/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:15:15 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/19 14:29:51 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}
/*
#include<fcntl.h>
int	main(int argc, char **argv)
{
	char	str[] = "feur";
	int	fd;

	fd = open(argv[1], O_WRONLY);
	ft_putend_fd(str, fd);
	close(fd);
	return (0);
}*/
