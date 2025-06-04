/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:05:01 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/04 18:58:54 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"
#include "../../include/execution.h"

int	only_redir(t_block_node *cmd, t_mem **mem)
{
	pid_t	pid;
	int		res;
	int		status;

	if (cmd->redirs_lst != NULL)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			res = pipe_fd_control_only_redir(cmd, mem);
			ft_clear_mem_and_exit(mem);
			exit(res);
		}
		waitpid(pid, &status, 0);
		return (print_child_statuses(NULL, &status));
	}
	return (0);
}

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
	return (print_child_statuses(NULL, &status));
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

	if (ft_lstsize(cmd->cmd_lst) == 1
		&& ft_strcmp(((t_cmd_node *)cmd->cmd_lst->content)->cmdvalue, "") == 0
		&& cmd->redirs_lst == NULL)
		return (0);
	ft_create_arr_and_expd(&cmd->cmd_lst, &cmd, mem);
	if (cmd->cmd_arr == NULL)
		return (only_redir(cmd, mem));
	else if (!is_built_in(cmd->cmd_arr))
		res = single_ext_cmd(ms_env, cmd, mem);
	else
		res = single_built_in(ms_env, cmd, mem);
	return (res);
}
