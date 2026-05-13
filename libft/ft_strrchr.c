/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:14:57 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/20 12:04:23 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	indice;

	i = 0;
	indice = -1;
	if (s[0] == '\0' && (unsigned char)c != '\0')
		return (0);
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			indice = i;
		i++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)&s[i]);
	if (indice == -1)
		return (0);
	return ((char *)&s[indice]);
}
/*
int	main(void)
{
	char	str[] = "\0";
	int	i = '\0';
	printf("s:%s\n", strrchr(str, i));
	printf("s2:%s", ft_strrchr(str, i));
	return (0);
}*/
