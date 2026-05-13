/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:04:00 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/13 15:27:42 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i++;
	}
}
/*
int	main(void)
{
	char	str[] = "feur";
	char	src[] = "feur";

	bzero(str, 1);
	ft_bzero(src, 1);
	printf("%c\n", str[0]);
	printf("%c", str[0]);
	return (0);
}*/
