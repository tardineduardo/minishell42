/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:08:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 13:50:15 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	execute_command(t_list **ms_env, t_block_node *cur_cmd, t_mem **mem)
{
	int		res;

	res = -1;
	if (!cur_cmd || !ms_env)
	{
		ft_dprintf(2, "cmd or ms_env: cmd executor: NULL pointer\n");
		exit(EXIT_FAILURE);
	}
	if (is_built_in(cur_cmd->cmd_arr))
		res = exec_built_in(ms_env, cur_cmd->cmd_arr, mem);
	else
		exec_external_cmd(ms_env, cur_cmd, mem);
	return (res);
}

int	print_child_statuses(t_pipe_data *p, int *status)
{
	int	last_index;
	int	last_status;

	if (p != NULL)
		last_index = p->num_cmds - 1;
	else
		last_index = 0;
	last_status = status[last_index];
	if (WIFSIGNALED(last_status))
		return (signal_statuses(last_index, status));
	else if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	else
		return (1);
}

int	ft_exec_subshell(t_list **ms_env, t_ast_node **root, t_mem **mem)
{
	int		ret;
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ret = ft_execute(ms_env, &(*root)->subshell->body, mem);
		exit(ret);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
}

int	ft_execute(t_list **ms_env, t_ast_node **root, t_mem **mem)
{
	int		res;

	res = -1;
	if (!root)
		return (0);
	if ((*root)->type == NODE_COMMAND)
		res = exec_single_cmd(ms_env, (*root)->block_node, mem);
	else if ((*root)->type == NODE_LOGICAL && (*root)->logical->op == OP_AND)
	{
		if (ft_execute(ms_env, &(*root)->logical->left, mem) == 0)
			return (ft_execute(ms_env, &(*root)->logical->right, mem));
		return (1);
	}
	else if ((*root)->type == NODE_LOGICAL && (*root)->logical->op == OP_OR)
	{
		if (ft_execute(ms_env, &(*root)->logical->left, mem) != 0)
			return (ft_execute(ms_env, &(*root)->logical->right, mem));
		return (0);
	}
	else if ((*root)->type == NODE_SUBSHELL)
		res = ft_exec_subshell(ms_env, root, mem);
	else if ((*root)->type == NODE_PIPELINE)
		res = exec_pipeline(ms_env, &(*root)->pipeline->cmds,
				(*root)->pipeline->cmd_count, mem);
	return (res);
}
