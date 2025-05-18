/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:49:50 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:06:14 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

// back means the end of the list, far from head.
void	ft_dlstadd_back(t_dlist **lst, t_dlist *new)
{
	t_dlist	*last;

	if (lst == NULL || new == NULL)
		return ;
	new->next = NULL;
	if (*lst == NULL)
	{
		new->prev = NULL;
		*lst = new;
	}
	else
	{
		last = ft_dlstlast(*lst);
		last->next = new;
		new->prev = last;
	}
}
