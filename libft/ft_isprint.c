/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:05:02 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/18 13:35:32 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

int	ft_isprint(int c)
{
	if (c < 32)
		return (0);
	if (c >= 127)
		return (0);
	return (1);
}
/*
int	main(void)
{
	int	i;
	i = 0;
	while (i <= 130)
	{
		printf("%d : %d\n", i, isprint(i));
		printf("%d : %d\n\n", i , ft_isprint(i));
		i++;
	}
	return (0);
}*/
