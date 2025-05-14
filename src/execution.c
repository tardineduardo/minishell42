/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:08:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/14 15:15:22 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/expand.h"
#include "../include/parsing.h"
#include "../include/environs.h"
#include "../include/readline.h"
#include "../include/builtins.h"
#include "../include/execution.h"

int	execute_command(t_list **ms_env, t_block_node *cur_cmd, t_mem **mem)
{
	int		res;

	res = -1;
	if (!cur_cmd || !ms_env)
	{
		ft_dprintf(2, "cmd or ms_env: cmd executor: NULL pointer\n");
		exit(EXIT_FAILURE);
	}
	cur_cmd->cmd_arr = ft_expand_cmd_arr(cur_cmd->cmd_arr, mem);
	if (is_built_in(cur_cmd->cmd_arr))
		res = exec_built_in(ms_env, cur_cmd->cmd_arr);
	else
		exec_external_cmd(ms_env, cur_cmd);
	return (res);
}

int	print_child_statuses(t_pipe_data *p, int *status)
{
	int	sig;
	int	index;
	int	i;
	int	res;

	i = 1;
	if (p != NULL)
		i = p->num_cmds;
	index = 0;
	res = 0;
	while (index < i)
	{
		if (WIFSIGNALED(status[index]))
		{
			sig = WTERMSIG(status[index]);
			if (sig == SIGQUIT)
				return (ft_printf("Quit (core dumped)\n"));
			if (sig == SIGINT)
				res = (sig + 128);
			return (res);
		}
		if (WIFEXITED(status[index]))
			res = WEXITSTATUS(status[index]);
		index++;
	}
	return (res);
}

int	ft_execute(t_list **ms_env, t_ast_node **root, t_mem **mem)
{
	int		res;
	int		ret;
	int		status;
	pid_t	pid;

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
	{
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
	else if ((*root)->type == NODE_PIPELINE)
		res = exec_pipeline(ms_env, &(*root)->pipeline->cmds, (*root)->pipeline->cmd_count, mem);
	return (res);
}
