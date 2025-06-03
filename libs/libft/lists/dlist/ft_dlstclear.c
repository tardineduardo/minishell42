/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstclear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:33:44 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 23:28:32 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	ft_dlstclear(t_dlist **lst, void (*del)(void*))
{
	t_dlist	*curr;
	t_dlist	*next;

	if (lst == NULL || *lst == NULL)
		return ;
	curr = *lst;
	while (curr)
	{
		next = curr->next;
		if (del)
			del(curr->content);
		free(curr);
		curr = next;
	}
	*lst = NULL;
}
