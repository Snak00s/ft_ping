/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 15:58:12 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/13 16:13:58 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		index;
	char	*str;

	i = 0;
	index = 0;
	str = (char *)malloc(((ft_strlen(s1) + ft_strlen(s2)) + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s1[i])
	{
		str[index] = s1[i];
		i++;
		index++;
	}
	i = 0;
	while (s2[i])
	{
		str[index] = s2[i];
		i++;
		index++;
	}
	str[index] = '\0';
	return (str);
}
/*
int	main(void)
{
	char	s1[] = "bien";
	char	s2[] = "le bonjour";
	char	*str = ft_strjoin(s1, s2);
	printf("%s", str);
	return(0);
}*/
