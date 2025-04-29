/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_ast_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:50 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/28 21:24:37 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exec_ast(t_list **ms_env, t_ast_node **root)
{
    if (!root) 
		return (0);
	if ((*root)->type == NODE_COMMAND)
		return (exec_single_cmd(ms_env, (*root)->cmd));
	else if ((*root)->type == NODE_LOGICAL && (*root)->logical->op == OP_AND)
	{
		if (exec_ast(ms_env, &(*root)->logical->left) == 0)
			return (exec_ast(ms_env, &(*root)->logical->right));
		return (1);
	}
	else if ((*root)->type == NODE_LOGICAL && (*root)->logical->op == OP_OR)
	{
		if (exec_ast(ms_env, &(*root)->logical->left) != 0)
			return (exec_ast(ms_env, &(*root)->logical->right));
		return (0);
	}
	else if ((*root)->type == NODE_SUBSHELL)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			int ret = exec_ast(ms_env, &(*root));
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
		return (exec_pipe(ms_env, &(*root)->pipeline->cmds, (*root)->pipeline->cmd_count));
    return (1);
}
