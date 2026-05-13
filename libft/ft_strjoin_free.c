/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:20:13 by tpinton           #+#    #+#             */
/*   Updated: 2026/04/27 16:31:52 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

char	*ft_strjoin_free(char *s1, char const *s2)
{
	int		i;
	int		index;
	char	*str;

	i = 0;
	index = 0;
	str = (char *)malloc(((ft_strlen(s1) + ft_strlen(s2)) + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s1 && s1[i])
	{
		str[index] = s1[i];
		i++;
		index++;
	}
	i = 0;
	while (s2 && s2[i])
	{
		str[index] = s2[i];
		i++;
		index++;
	}
	str[index] = '\0';
	free(s1);
	return (str);
}