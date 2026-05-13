/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:09:35 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/13 13:27:29 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
/*
#include<stdio.h>
#include<ctype.h>

int	main(void)
{
	int	i;
	i = '0';
	printf("%c", toupper(i));
	printf("%c", ft_toupper(i));
	return (0);
}*/
