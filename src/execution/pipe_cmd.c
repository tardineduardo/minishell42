/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:10:35 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 20:34:10 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

void	exec_child_core(t_pipe_data *p, t_list **ms_env,
	t_ast_node *cmd_node, t_mem **mem)
{
	t_block_node	*cmd;
	int				res;

	res = 0;
	signal_child_process();
	if (cmd_node->type == NODE_COMMAND)
	{
		cmd = cmd_node->block_node;
		res = pipe_fd_control_for_ast_node(p, cmd_node, p->pipefd, mem);
		ft_create_arr_and_expd(&cmd->cmd_lst, &cmd, mem);
		if (cmd->cmd_arr != NULL)
			res = execute_command(ms_env, cmd, mem);
	}
	else if (cmd_node->type == NODE_SUBSHELL)
	{
		res = pipe_fd_control_subshell(p, p->pipefd);
		res = ft_execute(ms_env, &cmd_node->subshell->body, mem);
	}
	if (cmd_node->type == NODE_COMMAND
		&& (cmd_node->block_node->cmd_arr == NULL
			|| is_built_in(cmd_node->block_node->cmd_arr)))
		ft_clear_mem_and_exit(mem);
	exit(res);
}

void	exec_child_pipe_cmd(t_pipe_data *p, t_list **ms_env,
	t_ast_node *cmd_node, t_mem **mem)
{
	pid_t			pid;
	int				res;

	res = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	p->child_pids[p->i] = pid;
	if (pid == 0)
		exec_child_core(p, ms_env, cmd_node, mem);
}

int	wait_for_all_children(t_pipe_data p)
{
	int	i;
	int	status;

	i = 0;
	if (p.prev_fd != 0)
		close(p.prev_fd);
	while (i < p.num_cmds)
	{
		if (p.child_pids[i] > 0)
		{
			waitpid(p.child_pids[i], &status, 0);
			p.status_arr[i] = status;
		}
		i++;
	}
	return (print_child_statuses(&p, p.status_arr));
}

int	exec_pipeline(t_list **env, t_list **cmds, int num_cmds, t_mem **mem)
{
	t_pipe_data	p;
	t_list		*node;

	ft_bzero(p.child_pids, sizeof(pid_t) * num_cmds);
	ft_bzero(p.status_arr, sizeof(pid_t) * num_cmds);
	p.prev_fd = 0;
	p.num_cmds = num_cmds;
	p.i = 0;
	node = *cmds;
	while (node && p.i < num_cmds)
	{
		if (p.i < num_cmds - 1 && pipe(p.pipefd) == -1)
			ft_handle_exec_error("pipe");
		exec_child_pipe_cmd(&p, env, (t_ast_node *)node->content, mem);
		if (p.i > 0)
			close(p.prev_fd);
		p.prev_fd = p.pipefd[0];
		close(p.pipefd[1]);
		p.i++;
		node = node->next;
	}
	return (wait_for_all_children(p));
}
