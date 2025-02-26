/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:49:21 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:07:17 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

int	ft_lstsize(t_list *lst)
{
	int		counter;
	t_list	*trav;

	trav = lst;
	counter = 0;
	while (trav != NULL)
	{
		trav = trav->next;
		counter++;
	}
	return (counter);
}
