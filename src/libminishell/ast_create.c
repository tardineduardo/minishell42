#include "./include/prototype.h"

t_ast_tree	*create_node(t_list *cmd_node)
{
	t_ast_tree		*new_root;
	t_cmd_node	*cmd;

	cmd = cmd_node;
	new_root = (t_ast_tree *)malloc(sizeof(t_ast_tree));
	if (!new_root)
		return (NULL);
	new_root->cmd = cmd->cmd_arr;
	new_root->type = CMD || PIPE || OPERATOR_OR || OPERATOR_AND || REDIRECT_IN || REDIRECT_OUT || APPEND || HEREDOC_IN;
	new_root->right = NULL;
	new_root->left = NULL;
}

t_ast_tree	*add_node()
{

}