#include "../libft.h"

t_btree *btree_create_node(void *data)
{
	t_btree *new;

	new = malloc(sizeof(t_btree));
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->data = data;
	return (new);
}
