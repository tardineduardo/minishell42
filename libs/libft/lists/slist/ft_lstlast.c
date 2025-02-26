/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:48:56 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:07:07 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*trav;

	if (lst == NULL)
		return (NULL);
	trav = lst;
	while (trav->next != NULL)
		trav = trav->next;
	return (trav);
}
