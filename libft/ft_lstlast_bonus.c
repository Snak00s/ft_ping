/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:23:25 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/22 11:51:40 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

t_list	*ft_lstlast(t_list	*lst)
{
	if (lst == NULL)
		return (0);
	while (lst->next)
	{
		lst = lst-> next;
	}
	return (lst);
}
/*
int	main(void)
{
	t_list	*first;
	t_list	*second;
	t_list	*third;

	first = ft_lstnew("1");
	second = ft_lstnew("2");
	third = ft_lstnew("3");
	
	ft_lstadd_front(&first, second);
	ft_lstadd_front(&first, third);
	printf("%s", (char *)ft_lstlast(third)->content);
	return(0);
}*/