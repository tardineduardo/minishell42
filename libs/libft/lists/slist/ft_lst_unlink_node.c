
#include "../../libft.h"

//removes node from a list, but does not free it
void ft_lst_unlink_node(t_list **head, t_list *node)
{
	t_list	*trav;
	t_list	*prev;

	if (*head == NULL || node == NULL)
		return ;
	if (*head == node)
	{
		*head = node->next;
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
	return ;
}
