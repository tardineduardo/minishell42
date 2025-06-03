/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:28:25 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 16:40:20 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"
#include "../../include/expand.h"
#include "../../include/execution.h"
#include <termios.h>

bool	save_termios(struct termios *saved)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, saved) == 0)
			return (true);
	}
	return (false);
}

bool	restore_termios(struct termios *saved)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcsetattr(STDIN_FILENO, TCSANOW, saved) == 0)
			return (true);
	}
	return (false);
}

int	pipe_fd_control(t_pipe_data *pipe_data, t_block_node *cur_cmd,
		int pipefd[2], t_mem **mem)
{
	struct termios	old_termios;
	int				res;
	bool			has_termios;

	has_termios = save_termios(&old_termios);
	res = redir_files_validation(&cur_cmd->redirs_lst, mem);
	if (cur_cmd->input_lst != NULL)
		fd_input_redir(&cur_cmd->input_lst, mem);
	else if (pipe_data->i > 0)
	{
		dup2(pipe_data->prev_fd, STDIN_FILENO);
		close(pipe_data->prev_fd);
	}
	if (cur_cmd->output_lst != NULL)
		fd_output_redir(&cur_cmd->output_lst, mem);
	else if (pipe_data->i < pipe_data->num_cmds - 1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (has_termios)
		restore_termios(&old_termios);
	return (res);
}

int	pipe_fd_control_single_cmd(t_block_node *cur_cmd, t_mem **mem)
{
	struct termios	old_termios;
	int				res;
	bool			has_termios;

	has_termios = save_termios(&old_termios);
	res = redir_files_validation(&cur_cmd->redirs_lst, mem);
	if (!is_built_in(cur_cmd->cmd_arr))
	{
		if (cur_cmd->input_lst != NULL)
			fd_input_redir(&cur_cmd->input_lst, mem);
	}
	if (cur_cmd->output_lst != NULL)
		fd_output_redir(&cur_cmd->output_lst, mem);
	if (has_termios)
		restore_termios(&old_termios);
	return (res);
}

int	pipe_fd_control_only_redir(t_block_node *cur_cmd, t_mem **mem)
{
	struct termios	old_termios;
	int				res;
	bool			has_termios;

	has_termios = save_termios(&old_termios);
	res = redir_files_validation(&cur_cmd->redirs_lst, mem);
	if (cur_cmd->input_lst != NULL)
		fd_input_redir(&cur_cmd->input_lst, mem);
	if (cur_cmd->output_lst != NULL)
		fd_output_redir(&cur_cmd->output_lst, mem);
	if (has_termios)
		restore_termios(&old_termios);
	return (res);
}
