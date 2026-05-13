/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:57:21 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/19 15:16:34 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

static int	ft_intlen(int n)
{
	int	i;
	int	c;

	i = n;
	c = 0;
	if (i < 0)
		c++;
	while (i != 0)
	{
		i /= 10;
		c++;
	}
	return (c);
}

static int	ft_power(int len)
{
	int	n;
	int	i;

	n = 1;
	i = 0;
	while (i < len - 1)
	{
		n *= 10;
		i++;
	}
	return (n);
}

static char	*ft_intmin(int n)
{
	char	*str;

	if (n == 0)
	{
		str = (char *)malloc(2 * sizeof(char));
		if (!str)
			return (NULL);
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	else
	{
		str = ft_itoa(n + 1);
		if (!str)
			return (NULL);
		str[10] = str[10] + 1;
		return (str);
	}
}

static char	*ft_alloc(int len)
{
	char	*str;

	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		len;
	int		power;

	i = 0;
	len = ft_intlen(n);
	if (n == -2147483648 || n == 0)
		return (ft_intmin(n));
	str = ft_alloc(len);
	if (!str)
		return (NULL);
	if (n < 0)
	{
		str[i++] = '-';
		n = n * -1;
	}
	power = ft_power(len - i);
	while (i < len)
	{
		str[i++] = '0' + (n / power);
		n = n - ((n / power) * power);
		power /= 10;
	}
	return (str[i] = '\0', str);
}
/*
int	main(void)
{
	char	*i1 = ft_itoa(-1234);
	if (!i1)
		return(0);
	printf("%s", i1);
	return(0);
}*/
