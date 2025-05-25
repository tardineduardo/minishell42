/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlst_destroy_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:33:44 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/06 22:46:50 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

static void	ft_handle_head(t_dlist **head, t_dlist *node, void (*del)(void*));

/* removes node from a list and frees it (the node and its content).
del can be NULL if no content inside the node needs to be freed.
this is a safe version, that returns NULL if node is not found in head */
t_dlist	*ft_dlst_destroy_node(t_dlist **head, t_dlist *node, void (*del)(void*))
{
	t_dlist	*trav;

	if (!head || !*head || !node)
		return (NULL);
	if (*head == node)
	{
		ft_handle_head(head, node, del);
		return (*head);
	}
	trav = *head;
	while (trav != NULL && trav != node)
		trav = trav->next;
	if (trav == NULL)
		return (NULL);
	if (trav->prev)
		trav->prev->next = trav->next;
	if (trav->next)
		trav->next->prev = trav->prev;
	if (del)
		(*del)(trav->content);
	free(trav);
	return (*head);
}

t_dlist	*ft_dlst_quick_destroy_node(t_dlist **head, t_dlist *node,
		void (*del)(void*))
{
	if (!head || !*head || !node)
		return (NULL);
	if (*head == node)
	{
		*head = node->next;
		if (node->next)
			node->next->prev = NULL;
		if (del)
			(*del)(node->content);
		free(node);
		return (*head);
	}
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (del)
		(*del)(node->content);
	free(node);
	return (*head);
}

static void	ft_handle_head(t_dlist **head, t_dlist *node, void (*del)(void*))
{
	*head = node->next;
	if (node->next)
		node->next->prev = NULL;
	if (del)
		(*del)(node->content);
	free(node);
	return ;
}
