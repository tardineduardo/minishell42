#include "./include/prototype.h"

exec_single()
{

}


exec_multi_cmd(t_ast **root)
{
	if ((*root)->type != CMD)
	{
		exec_multi_cmd(((*root))->left);
		exec_multi_cmd((*root)->right);
	}
	if ((*root)->inherited_type == OPERATOR_OR)
	{
		if (!exec_pipe((*root)->left))
		{
			exec_pipe((*root)->right);
		}
	}
	else if ((*root)->inherited_type == OPERATOR_AND)
	{
		if (exec_pipe((*root)->left))
		{
			exec_pipe((*root)->right);
		}
	}
	else if ((*root)->inherited_type == PIPE)
		exec_pipe((*root)->cmd);
	else
		return (NULL);
}