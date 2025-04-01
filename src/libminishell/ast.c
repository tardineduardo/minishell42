#include "./include/prototype.h"

t_ast	*create_node(char **cmd_arr)
{
	t_ast	*new_root;

	new_root = (t_ast *)malloc(sizeof(t_ast));
	if (!new_root)
		return (NULL);
	new_root->cmd = cmd_arr;
	new_root->type = NULL;
	new_root->right = NULL;
	new_root->left = NULL;
}

t_ast	*add_node()