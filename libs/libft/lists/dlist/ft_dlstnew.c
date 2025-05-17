/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:23:32 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:06:32 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_dlist	*ft_dlstnew(void *content)
{
	t_dlist	*a;

	a = malloc(sizeof(t_dlist));
	if (!a)
		return (NULL);
	a->content = content;
	a->next = NULL;
	a->prev = NULL;
	return (a);
}
