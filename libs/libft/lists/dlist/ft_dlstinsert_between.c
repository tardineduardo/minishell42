/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstinsert_between.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 14:51:03 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/08 14:51:08 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	ft_dlstinsert_between(t_dlist **lst, t_dlist *insert,
			t_dlist *start, t_dlist *end)
{
	t_dlist	*last;

	if (!insert)
		return ;
	last = insert;
	while (last->next)
		last = last->next;
	if (!start && !end)
	{
		*lst = insert;
		return ;
	}
	if (!start)
	{
		last->next = end;
		if (end)
			end->prev = last;
		*lst = insert;
		return ;
	}
	insert->prev = start;
	last->next = end;
	start->next = insert;
	if (end)
		end->prev = last;
}
