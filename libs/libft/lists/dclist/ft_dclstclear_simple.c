/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:33:44 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:06:58 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	ft_dclstclear_simple(t_dll **tail)
{
	t_dll	*temp;
	t_dll	*next_node;

	if (tail == NULL || *tail == NULL)
		return ;
	temp = (*tail)->next;
	while (temp != *tail)
	{
		next_node = temp->next;
		free(temp);
		temp = next_node;
	}
	free(*tail);
	*tail = NULL;
}
