/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:40:53 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/26 12:35:57 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

static int	count_word(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != c)
		{
			count++;
			while (str[i] != c && str[i])
				i++;
			if (str[i] == '\0')
				i--;
		}
		i++;
	}
	return (count);
}

static int	word_len(char const *str, char c)
{
	int	i;

	i = 0;
	while (*str != c && *str)
	{
		str++;
		i++;
	}
	return (i);
}

static void	ft_strcpy(char const *src, char *str, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		str[i] = *src;
		src++;
		i++;
	}
	str[i] = '\0';
}

static void	ft_free(char **tab, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;

	i = 0;
	tab = (char **)malloc((count_word(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (0);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s != '\0')
		{
			tab[i] = (char *)malloc((word_len(s, c) + 1) * sizeof(char));
			if (!tab[i])
				return (ft_free(tab, i), NULL);
			ft_strcpy(s, tab[i], word_len(s, c));
			i++;
			while (*s != c && *s)
				s++;
		}
	}
	tab[i] = NULL;
	return (tab);
}
/*
int	main(void)
{
	char	**tab;
	char	str[] = "   f e ff ef    ef ef eefefefe  fef  ";
	int	i;

	i = 0;
	tab = ft_split(str, ' ');
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		free(tab[i]);
		i++;
	}
	free(tab);
	return(0);
}*/
