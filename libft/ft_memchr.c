/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:57:08 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/20 12:40:36 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((char *)s)[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	int	i = 'b';
	char	str[] = "bonjour";
	int	n = 4;
	char	*out = ft_memchr(str, i, n);
	//char	*u = memchr(str, i, n);
	printf("%s\n", out);
	//printf("%s", u);
	return (0);
}*/
