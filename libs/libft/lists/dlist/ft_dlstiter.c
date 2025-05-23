/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:36:34 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:07:05 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	ft_dlstiter(t_dlist *lst, void (*f)(void *))
{
	t_dlist	*temp;

	temp = lst;
	while (temp != NULL)
	{
		(*f)((void *)temp->content);
		temp = temp->next;
	}
}
