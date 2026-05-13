/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:44:34 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/22 12:02:57 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = ft_lstlast(*lst);
	temp->next = new;
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
	
	ft_lstadd_back(&first, second);
	ft_lstadd_back(&first, third);
	while(first)
	{
		printf("%s\n", (char *)first->content);
		first = first->next;
	}
	return(0);
}*/