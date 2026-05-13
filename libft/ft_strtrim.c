/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:15:05 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/20 13:24:31 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

static int	ft_charin(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	ft_start(char *str, char *set)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_charin(set, str[i]) == 0)
			return (i);
		i++;
	}
	return (i);
}

static int	ft_end(char *str, char *set)
{
	int	i;
	int	len;

	i = 1;
	len = 0;
	while (str[len])
		len++;
	while (len - i > 0)
	{
		if (ft_charin(set, str[len - i]) == 0)
			return (len - i);
		i++;
	}
	return (len - i);
}

static char	*ft_empty(int i)
{
	char	*str;

	str = (char *)malloc((i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[0] = '\0';
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		i;
	int		start;
	int		end;

	i = 0;
	start = ft_start((char *)s1, (char *)set);
	end = ft_end((char *)s1, (char *)set);
	if (start > end || (*s1 == '\0' && *set == '\0'))
	{
		str = ft_empty(i);
		if (!str)
			return (NULL);
		return (str);
	}
	str = (char *)malloc((end - start + 2) * sizeof(char));
	if (!str)
		return (NULL);
	while (start + i <= end)
	{
		str[i] = s1[start + i];
		i++;
	}
	str[i] = '\0';
	return ((char *)str);
}
/*
int	main(void)
{
	char	str[] = "dwbhudb hbudyw ";
	char	*strim;
	if (!(strim = ft_strtrim(str, " ")))
		printf("%s\n", "feur");
	else
		printf("%s\n", strim);
	return (0);
}*/
