/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:33:44 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/06 22:46:50 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp1;
	t_list	*temp2;

	if (lst == NULL || *lst == NULL)
		return ;
	temp1 = *lst;
	while (temp1 != NULL)
	{
		temp2 = temp1->next;
		if (temp1->content)
			if (del)
				(*del)(temp1->content);
		free(temp1);
		temp1 = temp2;
	}
	*lst = NULL;
}

void	ft_lstclear2(t_list **lst, void (*del)(void**))
{
	t_list	*temp1;
	t_list	*temp2;

	if (lst == NULL || *lst == NULL)
		return ;
	temp1 = *lst;
	while (temp1 != NULL)
	{
		temp2 = temp1->next;
		if (temp1->content)
			(*del)(&(temp1->content));
		free(temp1);
		temp1 = temp2;
	}
	*lst = NULL;
}
