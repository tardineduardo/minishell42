/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:05:01 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/28 16:38:44 by luide-ca         ###   ########.fr       */
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

int	single_ext_cmd(t_list **ms_env, t_block_node *cmd, t_mem **mem)
{
	pid_t	pid;
	int		status;
	int		res;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		signal_child_process();
		res = pipe_fd_control_single_cmd(cmd, mem);
		if (res != 0 && res == 255)
			exit(res);
		execute_command(ms_env, cmd, mem);
		exit(EXIT_SUCCESS);
	}
	signal_before_wait();
	waitpid(pid, &status, 0);
	signal_after_wait();
	res = print_child_statuses(NULL, &status);
	return (res);
}

int	single_built_in(t_list **ms_env, t_block_node *cmd, t_mem **mem)
{
	int		result;
	int		res;
	int		saved_stdout;

	result = 0;
	saved_stdout = -1;
	if (cmd->output_lst != NULL)
		saved_stdout = dup(STDOUT_FILENO);
	if (cmd->redirs_lst != NULL)
		result = pipe_fd_control_single_cmd(cmd, mem);
	if (result == 0)
		res = execute_command(ms_env, cmd, mem);
	else
		res = 1;
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	return (res);
}

int	exec_single_cmd(t_list **ms_env, t_block_node *cmd, t_mem **mem)
{
	int		res;

	if (cmd->cmd_arr[0] == NULL)
	{
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0) {
			int res = pipe_fd_control_only_redir(cmd, mem);
			ft_clear_mem_and_exit(mem);
			exit(res);
		}
		int status;
		waitpid(pid, &status, 0);
		return print_child_statuses(NULL, &status);
	}
	if (!is_built_in(cmd->cmd_arr))
		res = single_ext_cmd(ms_env, cmd, mem);
	else
		res = single_built_in(ms_env, cmd, mem);
	return (res);
}
