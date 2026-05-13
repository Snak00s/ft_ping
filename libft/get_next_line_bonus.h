/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:09:30 by tpinton           #+#    #+#             */
/*   Updated: 2026/04/17 17:00:42 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include<stdio.h>
# include<fcntl.h>
# include<stdlib.h>
# include<unistd.h>
# include<stdint.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

char	*get_next_line(int fd);
// char	*ft_strjoin(char *s1, char *s2);
char	*ft_buffmanage(char *str, char *temp, int rv, int fd);
int		ft_check(char *str);
char	*ft_newstr(char *str, char *line);
void	ft_bzero(void *s, size_t n);
char	*ft_line(char *str);
size_t	ft_len(const char *str, int mode);
void	ft_free(char *s1, char *vide);
int		ft_issue(int fd, char *str);

#endif