/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:57:46 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:07:11 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

t_dlist	*ft_dlstmap(t_dlist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_dlist	*node;
	t_dlist	*new;
	void	*new_content;

	if (!lst || !f || !del)
		return (NULL);
	new = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		node = ft_dlstnew(new_content);
		if (!node)
		{
			del(new_content);
			ft_dlstclear(&new, del);
			return (NULL);
		}
		ft_dlstadd_back(&new, node);
		lst = lst->next;
	}
	return (new);
}
