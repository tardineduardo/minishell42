
#include "../../libft.h"

/* removes node from a list and frees it (the node and its content).
del can be NULL if no content inside the node needs to be freed. */
void ft_lst_destroy_node(t_list **head, t_list *node, void (*del)(void*))
{
	t_list	*trav;
	t_list	*prev;

	if (*head == NULL || node == NULL)
		return ;
	if (*head == node)
	{
		*head = node->next;
		if (del)
			(*del)((void *)node->content);
		free(node);
		return ;
	}
	trav = *head;
	while (trav != NULL && trav != node)
	{
		prev = trav;
		trav = trav->next;
	}
	if (trav == NULL)
		return ;
	prev->next = trav->next;
	if (del)
		(*del)((void *)trav->content);
	free(trav);
}
