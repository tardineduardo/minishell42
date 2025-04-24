/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:44:56 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/24 13:29:51 by luide-ca         ###   ########.fr       */
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

void exec_pipe_cmd(t_pipe_data *p, t_list **env, t_cmd_node *cmd)
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
		pipe_fd_control(p, cmd, p->pipefd);
		exec_cmd(env, cmd);
		exit(EXIT_SUCCESS);
	}
}

int exec_pipe(t_list **env, t_list **org_token, int num_cmds)
{
	t_pipe_data p = { .prev_fd = 0, .num_cmds = num_cmds, .i = 0 };
	memset(p.child_pids, 0, sizeof(pid_t) * num_cmds);
	t_list *node = *org_token;
	int status;

	while (node && p.i < num_cmds)
	{
		t_cmd_node *tok = node->content;
		// Cria um novo pipe se houver próximo comando
		if (p.i < num_cmds - 1 && pipe(p.pipefd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		exec_pipe_cmd(&p, env, tok);
		// Fecha descritores não usados no pai
		if (p.i > 0)
			close(p.prev_fd);
		p.prev_fd = p.pipefd[0];
		close(p.pipefd[1]);
		p.i++;
		node = node->next;
	}
	if (p.i == p.num_cmds && p.prev_fd != 0)
		close(p.prev_fd);
	for (int i = 0; i < num_cmds; i++) {
		if (p.child_pids[i] > 0)
		{
			waitpid(p.child_pids[i], &status, 0);
		}
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else 
		return (-1);
}
