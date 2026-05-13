/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:45:10 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/18 14:58:25 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	*dtr;

	i = 0;
	str = (unsigned char *)src;
	dtr = (unsigned char *)dest;
	if (!dest && !src)
		return (dest);
	if (dtr > str)
	{
		while (n-- > 0)
		{
			dtr[n] = str[n];
		}
	}
	else
	{
		while (i < n)
		{
			dtr[i] = str[i];
			i++;
		}
	}
	return (dtr);
}
