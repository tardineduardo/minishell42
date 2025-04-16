#include "./include/prototype.h"

// Node types
typedef enum {
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_LOGICAL,
	NODE_SUBSHELL
} node_type;

// Logical operators
typedef enum {
	OP_NONE,
	OP_AND,
	OP_OR
} logical_op;

typedef struct s_ast_node
{
	node_type			type;
	logical_op			op;
	t_list				*cmd_lst;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*body;
}	t_ast_node;

int exec_ast(t_list **ms_env, t_ast_node **node)
{
    if (!node) 
		return (0);
	if ((*node)->type = NODE_COMMAND)
		return (exec_cmd(ms_env, (*node)->cmd_lst));
	else if ((*node)->type = NODE_LOGICAL && (*node)->op == OP_AND)
	{
		if (exec_ast(ms_env, (*node)->left) == 0)
			return (exec_ast(ms_env, (*node)->right));
		return (1);
	}
	else if ((*node)->type = NODE_LOGICAL && (*node)->op == OP_OR)
	{
		if (exec_ast(ms_env, (*node)->left) != 0)
			return (exec_ast(ms_env, (*node)->right));
		return (0);
	}
	else if ((*node)->type = NODE_SUBSHELL)
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
	else if ((*node)->type = NODE_PIPELINE)
		return (exec_pipe(ms_env, (*node)->cmd_lst));
    return (1);
}
