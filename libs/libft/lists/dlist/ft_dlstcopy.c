/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstcopy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:48:40 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:06:50 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_dlist	*ft_dlstcopy(t_dlist *source)
{
	t_dlist	*copy;
	t_dlist	*trav;
	t_dlist	*new_node;

	if (!source)
		return (NULL);
	copy = NULL;
	trav = source;
	while (trav)
	{
		new_node = ft_dlstnew(trav->content);
		if (!new_node)
			return (ft_dlstclear(&copy, free), NULL);
		ft_dlstadd_back(&copy, new_node);
		trav = trav->next;
	}
	return (copy);
}
