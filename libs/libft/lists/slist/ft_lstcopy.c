/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstcopy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:54:43 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:07:00 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_list	*ft_lstcopy(t_list *source)
{
	t_list	*copy;
	t_list	*trav;
	t_list	*new_node;

	if (!source)
		return (NULL);
	copy = NULL;
	trav = source;
	while (trav)
	{
		new_node = ft_lstnew(trav->content);
		if (!new_node)
			return (ft_lstclear(&copy, free), NULL);
		ft_lstadd_back(&copy, new_node);
		trav = trav->next;
	}
	return (copy);
}
