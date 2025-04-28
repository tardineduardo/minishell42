
#include "../../libft.h"


/* safe version, returns NULL if params are NULL or node is not found in head.
returns a pointer to head (or the new head) when node is found and removed */
t_dlist	*ft_dlst_unlink_node(t_dlist **head, t_dlist *node)
{
	t_dlist	*trav;

	if (!head || !*head || !node)
		return (NULL);
	if (*head == node)
	{
		*head = node->next;
		if (node->next)
			node->next->prev = NULL;
		return (head);
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
	return (head);
}

/* quick version, does not check if node is in head. returns NULL if params
are NULL. returns pointer to head (or the new head) after node is removed */
t_dlist	*ft_dlst_quick_unlink_node(t_dlist **head, t_dlist *node)
{
	if (!head || !*head || !node)
		return (NULL);
	if (*head == node)
	{
		*head = node->next;
		if (node->next)
			node->next->prev = NULL;
		return (head);
	}
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	return (head);
}
