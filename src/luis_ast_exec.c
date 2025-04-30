/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_ast_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:50 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/30 13:33:25 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exec_ast(t_list **ms_env, t_ast_node **root, t_mem **mem)
{
	int	res = -1;

    if (!root) 
		return (0);
	if ((*root)->type == NODE_COMMAND)
		res = exec_single_cmd(ms_env, (*root)->cmd, mem);
	else if ((*root)->type == NODE_LOGICAL && (*root)->logical->op == OP_AND)
	{
		if (exec_ast(ms_env, &(*root)->logical->left, mem) == 0)
			return (exec_ast(ms_env, &(*root)->logical->right, mem));
		return (1);
	}
	else if ((*root)->type == NODE_LOGICAL && (*root)->logical->op == OP_OR)
	{
		if (exec_ast(ms_env, &(*root)->logical->left, mem) != 0)
			return (exec_ast(ms_env, &(*root)->logical->right, mem));
		return (0);
	}
	else if ((*root)->type == NODE_SUBSHELL)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			int ret = exec_ast(ms_env, &(*root), mem);
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
	else if ((*root)->type == NODE_PIPELINE)
		res = execute_pipeline(ms_env, &(*root)->pipeline->cmds, (*root)->pipeline->cmd_count, mem);
    return (res);
}
