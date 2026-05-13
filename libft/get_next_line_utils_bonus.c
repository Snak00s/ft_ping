/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:09:33 by tpinton           #+#    #+#             */
/*   Updated: 2024/12/09 13:39:32 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"get_next_line_bonus.h"

size_t	ft_len(const char *str, int mode)
{
	size_t	i;

	i = 0;
	if (mode == 1)
	{
		if (!str)
			return (0);
		while (str[i])
			i++;
		return (i);
	}
	if (mode == 2)
	{
		if (!str)
			return (0);
		while (str[i])
		{
			if (str[i] == '\n')
				return (i + 1);
			i++;
		}
	}
	return (i);
}

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

int	ft_check(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i + 1);
		i++;
	}
	return (-1);
}

char	*ft_newstr(char *str, char *line)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	tmp = NULL;
	str += ft_len(line, 1);
	tmp = (char *)malloc((ft_len(str, 1) + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	while (str[i])
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	str -= ft_len(line, 1);
	ft_free(str, "");
	return (tmp);
}

char	*ft_line(char *str)
{
	char	*newline;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	newline = (char *)malloc((ft_len(str, 2) + 1) * sizeof(char));
	if (!newline)
		return (NULL);
	while (str[i] && str[i] != '\n')
	{
		newline[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		newline[i] = '\n';
		newline[i + 1] = '\0';
	}
	else
		newline[i] = '\0';
	return (newline);
}
