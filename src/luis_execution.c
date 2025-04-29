/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:44:56 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/28 19:31:42 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_cmd(t_list **ms_env, t_cmd_node *cur_cmd)
{
	char	**cmd_arr;

	if (!cur_cmd || !ms_env)
	{
		perror("cmd or ms_env: cmd executor");
		exit(EXIT_FAILURE);
	}
	cmd_arr = cur_cmd->cmd_arr;
	if (is_built_in(cmd_arr))
		exec_built_in(ms_env, cmd_arr);
	else
		exec_external_cmd(ms_env, cur_cmd);
	return (0);
}

void exec_pipe_cmd(t_pipe_data *p, t_list **ms_env, t_cmd_node *cmd)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	p->child_pids[p->i] = pid;
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		pipe_fd_control(p, cmd, p->pipefd);
		exec_cmd(ms_env, cmd);
		exit(EXIT_SUCCESS);
	}
}

int	exec_single_cmd(t_list **ms_env, t_cmd_node *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		exec_cmd(ms_env, cmd);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else 
		return (-1);
	return (0);
}

int	wait_and_status(t_pipe_data p)
{
	int	i;
	int	status;

	i = 0;
	if (p.i == p.num_cmds && p.prev_fd != 0)
		close(p.prev_fd);
	while (i < p.num_cmds)
	{
		if (p.child_pids[i] > 0)
			waitpid(p.child_pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else 
		return (-1);
}

int exec_pipe(t_list **env, t_list **org_token, int num_cmds)
{
	t_pipe_data p;
	t_list *node;
	
	ft_bzero(p.child_pids, sizeof(pid_t) * num_cmds);
	node = *org_token;
	p.prev_fd = 0;
	p.num_cmds = num_cmds;
	p.i = 0;
	while (node && p.i < num_cmds)
	{
		if (p.i < num_cmds - 1 && pipe(p.pipefd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		exec_pipe_cmd(&p, env, (t_cmd_node *)node->content);
		if (p.i > 0)
			close(p.prev_fd);
		p.prev_fd = p.pipefd[0];
		close(p.pipefd[1]);
		p.i++;
		node = node->next;
	}
	return(wait_and_status(p));
}
