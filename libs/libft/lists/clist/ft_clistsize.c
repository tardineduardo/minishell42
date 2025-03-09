/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clistsize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:39:36 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 20:03:29 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

int	ft_clstsize(t_list **tail)
{
	int		counter;
	t_list	*trav;

	if (*tail == NULL)
		return (0);
	trav = *tail;
	counter = 1;
	trav = trav->next;
	while (trav != *tail)
	{
		counter++;
		trav = trav->next;
	}
	return (counter);
}
