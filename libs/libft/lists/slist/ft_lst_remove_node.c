
#include "../../libft.h"

void ft_lst_remove_node(t_list **head, t_list *node, void (*del)(void*))
{
	t_list	*trav;
	t_list	*prev;

	if (*head == NULL || node == NULL)
		return ;
	if (*head == node)
	{
		*head = node->next;
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
	(*del)((void *)trav->content);
	free(trav);
}
