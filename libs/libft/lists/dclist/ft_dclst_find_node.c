/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dclst_find_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:23:12 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/12 18:23:48 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_dll	*ft_dclst_find_node(t_dll **tail, t_dll *node)
{
	t_dll	*trav;

	if (!tail || !(*tail) || !node)
		return (NULL);
	trav = *tail;
	while (1)
	{
		if (trav == node)
			return (trav);
		trav = trav->next;
		if (trav == *tail)
			break ;
	}
	return (NULL);
}
