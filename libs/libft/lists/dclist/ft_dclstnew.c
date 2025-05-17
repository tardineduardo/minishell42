/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dclstnew.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:18:44 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/12 18:18:48 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_dll	*ft_dclstnew(void *content)
{
	t_dll	*temp;

	temp = malloc(sizeof(t_dll));
	if (!temp)
		return (NULL);
	temp->content = content;
	temp->next = temp;
	temp->prev = temp;
	return (temp);
}
