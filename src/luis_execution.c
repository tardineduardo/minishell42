/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:44:56 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/22 15:06:24 by luide-ca         ###   ########.fr       */
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
	printf("passou aqui\n");
	if (is_built_in(cmd_arr))
		exec_built_in(ms_env, cmd_arr);
	else
		exec_external_cmd(ms_env, cur_cmd);
	return (0);
}

void exec_pipe_cmd(t_pipe_data *p, t_list **env, t_cmd_node *cmd)
{
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	p->child_pids[p->i] = pid;
	if (pid == 0) {
		//fprintf(stderr, "Child %d: Running command '%s'\n", p->i, cmd->cmd_arr[0]);
		pipe_fd_control(p, cmd, p->pipefd);
		//fprintf(stderr, "Child %d: FDs set up, executing\n", p->i);
		exec_cmd(env, cmd);
		perror("execve");
		exit(127);
	}
}

int exec_pipe(t_list **env, t_list **org_token, int num_cmds)
{
	t_pipe_data p = { .prev_fd = 0, .num_cmds = num_cmds, .i = 0 };
	memset(p.child_pids, 0, sizeof(pid_t) * num_cmds);
	t_list *node = *org_token;
	int status;

	if (/*p.i < num_cmds - 1 && */pipe(p.pipefd) == -1)
	{
				perror("pipe");
				exit(1);
	}
	while (node && p.i < num_cmds)
	{
		t_org_tok *tok = node->content;
		if (tok->cmd != -1) {
			t_cmd_node *cmd = tok->cmd_node;

			// Cria um novo pipe se houver próximo comando
			

			exec_pipe_cmd(&p, env, cmd);
			// Fecha descritores não usados no pai
			if (p.i > 0) // Parent process
				close(p.prev_fd);
			p.prev_fd = p.pipefd[0]; // Store read end for next command
			close(p.pipefd[1]);  // Close write end
			p.i++;
		}
		else
		{
			if (p.prev_fd != STDIN_FILENO)
				close(p.prev_fd);
			if (p.i < p.num_cmds - 1) {
				close(p.pipefd[1]);
				p.prev_fd = p.pipefd[0];
			}
		}
		node = node->next;
	}
	for (int i = 0; i < num_cmds; i++) {
		if (p.child_pids[i] > 0) {
			waitpid(p.child_pids[i], &status, 0);
			printf("%d\n", status);
		}
	}
	return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}