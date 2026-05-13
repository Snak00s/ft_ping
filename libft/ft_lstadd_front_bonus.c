/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpinton <tpinton@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:37:47 by tpinton           #+#    #+#             */
/*   Updated: 2024/11/21 17:25:38 by tpinton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}
/*
int main(void)
{
	t_list  *front;
	t_list  *new;

	front = ft_lstnew("1er");
	new = ft_lstnew("2eme");
	printf("%s : %s\n", (char *)front->content, (char *)new->content);
	ft_lstadd_front(&front, new);
	printf("%s : %s\n", (char *)front->content, (char *)new->next->content);
	return (0);
}*/