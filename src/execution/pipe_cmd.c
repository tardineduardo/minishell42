/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:10:35 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/31 20:10:16 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/expand.h"
#include "../../include/parsing.h"
#include "../../include/environs.h"
#include "../../include/readline.h"
#include "../../include/builtins.h"
#include "../../include/execution.h"

void	execute_child_pipe_command(t_pipe_data *p, t_list **ms_env, t_block_node *cmd, t_mem **mem)
{
	pid_t	pid;
	int		res;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	p->child_pids[p->i] = pid;
	if (pid == 0)
	{
		signal_child_process();
		pipe_fd_control(p, cmd, p->pipefd, mem);
		ft_create_cmd_arr_and_expand(&cmd->cmd_lst, cmd, mem);
		res = execute_command(ms_env, cmd, mem);
		if (is_built_in(cmd->cmd_arr))
			ft_clear_mem_and_exit(mem);
		exit(res);
	}
}

int	wait_for_all_children(t_pipe_data p)
{
	int	i;
	int	status;

	i = 0;
	if (p.i == p.num_cmds && p.prev_fd != 0)
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
		{
			perror("pipe");
			exit(1);
		}
		execute_child_pipe_command(&p, env, (t_block_node *)node->content, mem);
		if (p.i > 0)
			close(p.prev_fd);
		p.prev_fd = p.pipefd[0];
		close(p.pipefd[1]);
		p.i++;
		node = node->next;
	}
	return (wait_for_all_children(p));
}
