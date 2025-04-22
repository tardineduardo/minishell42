/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:44:56 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/22 11:27:48 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include "../include/minishell.h"

int	exec_cmd(t_list **ms_env, t_cmd_node *cur_cmd)
{
	char	**cmd_arr;

	cmd_arr = cur_cmd->cmd_arr;
	if (!cur_cmd || !ms_env)
	{
		perror("cmd or ms_env: cmd executor");
		exit(EXIT_FAILURE);
	}
	if (is_built_in(cmd_arr))
	{
		exec_built_in(ms_env, cmd_arr);
		return (0);
	}
	else
	{
		exec_external_cmd(ms_env, cur_cmd);
		return (0);
	}
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
		// Redireciona entrada se não for o primeiro comando
		// if (p->i > 0) {
		// 	dup2(p->prev_fd, STDIN_FILENO);
		// 	close(p->prev_fd);
		// }

		// // Redireciona saída se não for o último comando
		// if (p->i < p->num_cmds - 1) {
		// 	dup2(p->pipefd[1], STDOUT_FILENO);
		// }

		// Fecha os pipes desnecessários
		// close(p->pipefd[0]);
		//close(p->pipefd[1]);

		// Redirecionamento de arquivos (opcional)
		// if (cmd->input_lst)
		// 	dup2(file_input_handler(&cmd->input_lst), STDIN_FILENO);
		// if (cmd->output_lst)
		// 	dup2(file_output_handler(&cmd->output_lst), STDOUT_FILENO);
		pipe_fd_control(p, cmd, p->pipefd);
		// Execução do comando
		//char **cmd_arr = update_cmd_arr(env, cmd->cmd_arr);
		//char **env_arr = ft_ms_env_arr(env);

		exec_cmd(env, cmd);

		perror("execve");
		exit(127);
	}
}

	int exec_pipe(t_list **env, t_list **org_token, int num_cmds)
	{
		t_pipe_data p = { .prev_fd = -1, .num_cmds = num_cmds, .i = 0 };
		memset(p.child_pids, 0, sizeof(pid_t) * num_cmds);
		t_list *node = *org_token;
		int status;

		while (node && p.i < num_cmds) {
			t_org_tok *tok = node->content;
			if (tok->cmd != -1) {
				t_cmd_node *cmd = tok->cmd_node;

				// Cria um novo pipe se houver próximo comando
				if (p.i < num_cmds - 1 && pipe(p.pipefd) == -1)
				{
					perror("pipe");
					exit(1);
				}

				exec_pipe_cmd(&p, env, cmd);

				// Fecha descritores não usados no pai
				if (p.prev_fd != -1)
					close(p.prev_fd);
				if (p.i < num_cmds - 1)
					p.prev_fd = p.pipefd[0]; // salva para o próximo comando
				close(p.pipefd[1]); // sempre fecha a escrita no pai

				p.i++;
			}
			node = node->next;
		}
		for (int i = 0; i < num_cmds; i++) {
			if (p.child_pids[i] > 0) {
				waitpid(p.child_pids[i], &status, 0);
			}
		}
		return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
		//return (0);
	}