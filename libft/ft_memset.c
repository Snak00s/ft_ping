/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:08:28 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/13 17:40:33 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = c;
		i++;
	}
	return (s);
}
/*
int	main(void)
{
	char	feur[] = "Bien le bonjour";
	char	str[] = "Bien le bonjour";
	memset(str, '#', 10);
	ft_memset(feur, '#', 10);
	printf("%s\n", str);
	printf("%s", feur);
	return (0);
}*/
