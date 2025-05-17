/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:48:40 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:06:50 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

// front means the start of the list, right after head.
void	ft_dlstadd_front(t_dlist **lst, t_dlist *new)
{
	new->next = *lst;
	*lst = new;
}
