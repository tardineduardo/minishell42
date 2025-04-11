#include "./include/prototype.h"

int exec_ast(t_list **ms_env, t_ast_tree **node)
{
    if (!node) 
		return (0);
	if ((*node)->type = CMD)
		return (exec_cmd(ms_env, (*node)->cmd));
	else if ((*node)->type = OPERATOR_AND)
	{
		if (exec_ast(ms_env, (*node)->left) == 0)
			return (exec_ast(ms_env, (*node)->right));
		return (1);
	}
	else if ((*node)->type = OPERATOR_OR)
	{
		if (exec_ast(ms_env, (*node)->left) != 0)
			return (exec_ast(ms_env, (*node)->right));
		return (0);
	}
	else if ((*node)->type = SUBSHELL)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			int ret = exec_ast(ms_env, (*node));
			exit(ret);
		}
		else
		{
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
			return (1);
		}
	}
	else if ((*node)->type = PIPE)
		return (exec_pipe(ms_env, node));
    return (1);
}
