/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:23:30 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/22 12:47:00 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	del(lst->content);
	free(lst);
}
/*
void	del(void *content)
{
	if (content)
	{
		//content = NULL;
		free(content);
	}
}

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
	ft_lstdelone(third, del);
	while(first)
	{
		printf("%s", (char *)first->content);
		first = first->next;
	}
	return(0);
}*/