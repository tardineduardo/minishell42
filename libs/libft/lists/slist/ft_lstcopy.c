#include "../../libft.h"

t_list	*ft_lstcopy(t_list *source)
{
	t_list	*copy;
	t_list	*trav;
	t_list	*new_node;

	if (!source)
		return (NULL);
	copy = NULL;
	trav = source;
	while (trav)
	{
		new_node = ft_lstnew(trav->content);
		if (!new_node)
			return (ft_lstclear(&copy, free), NULL);
		ft_lstadd_back(&copy, new_node);
		trav = trav->next;
	}
	return (copy);
}
