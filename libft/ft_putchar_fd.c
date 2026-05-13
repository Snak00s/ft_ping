/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:49:15 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/14 15:57:07 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/*
#include<fcntl.h>
int	main(int argc, char **argv)
{
	int	fd;

	argc++;
	fd = open(argv[1], O_WRONLY);
	ft_putchar_fd('o', fd);
	return (0);
}*/
