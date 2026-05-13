/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:09:26 by tpinton           #+#    #+#             */
/*   Updated: 2026/04/17 17:00:35 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"get_next_line_bonus.h"

static char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		index;
	char	*str;

	i = 0;
	index = 0;
	str = (char *)malloc(((ft_len(s1, 1) + ft_len(s2, 1)) + 1) * sizeof(char));
	if (!str)
		return (NULL);
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
	return (ft_free(s1, ""), str);
}

void	ft_free(char *s1, char *vide)
{
	if (s1 != vide)
	{
		free(s1);
		s1 = NULL;
	}
}

char	*ft_buffmanage(char *str, char *temp, int rv, int fd)
{
	if (!str && rv != 0)
	{
		str = (char *)malloc(1 * sizeof(char));
		if (!str)
			return (NULL);
		ft_bzero(str, 1);
	}
	while (ft_check(temp) == -1 && rv > 0)
	{
		str = ft_strjoin(str, temp);
		if (!str)
			return (NULL);
		ft_bzero(temp, BUFFER_SIZE + 1);
		rv = read(fd, temp, BUFFER_SIZE);
	}
	if (ft_check(temp) > -1 && rv > 0)
	{
		str = ft_strjoin(str, temp);
		if (!str)
			return (NULL);
	}
	return (ft_free(temp, ""), str);
}

int	ft_issue(int fd, char *str)
{
	if (read(fd, 0, 0) == -1 || fd < 0 || BUFFER_SIZE <= 0)
	{
		if (str)
		{
			ft_free(str, "");
			str = NULL;
		}
		return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*str = NULL;
	char		*line;
	char		*temp;
	int			rv;

	if (ft_issue(fd, str) == 0)
	{
		return (str = NULL, NULL);
	}
	temp = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	ft_bzero(temp, BUFFER_SIZE + 1);
	rv = read(fd, temp, BUFFER_SIZE);
	str = ft_buffmanage(str, temp, rv, fd);
	if (!str)
		return (NULL);
	if (str[0] == '\0' && rv == 0)
	{
		ft_free(str, "");
		return (str = NULL, NULL);
	}
	line = ft_line(str);
	str = ft_newstr(str, line);
	return (line);
}
