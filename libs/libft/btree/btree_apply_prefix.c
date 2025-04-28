#include "../libft.h"

void btree_apply_prefix(t_btree *root, void (*applyf)(void *))
{
	if (!root || !applyf)
		return ;
	(*applyf)(root->data);
	btree_apply_prefix(root->left, applyf);
	btree_apply_prefix(root->right, applyf);
	return ;
}
