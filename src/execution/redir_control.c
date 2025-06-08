/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:28:25 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 17:34:21 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	pipe_fd_control_subshell(t_pipe_data *pipe_data, int pipefd[2])
{
	if (pipe_data->i > 0)
	{
		dup2(pipe_data->prev_fd, STDIN_FILENO);
		close(pipe_data->prev_fd);
	}
	if (pipe_data->i < pipe_data->num_cmds - 1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	else
	{
		if (pipefd[1] != -1)
			close(pipefd[1]);
		if (pipefd[0] != -1)
			close(pipefd[0]);
	}
	return (0);
}

int	pipe_core_fd_control(t_pipe_data *pipe_data, t_block_node *cur_cmd,
		int pipefd[2], t_mem **mem)
{
	struct termios	old_termios;
	int				res;
	bool			has_termios;

	has_termios = save_termios(&old_termios);
	res = redir_files_validation(&cur_cmd->redirs_lst, mem, false);
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

int	pipe_fd_control_for_ast_node(t_pipe_data *p, t_ast_node *cmd_node,
		int pipefd[2], t_mem **mem)
{
	int				res;
	t_block_node	*cur_cmd;

	if (cmd_node->type != NODE_COMMAND)
		return (pipe_fd_control_subshell(p, pipefd));
	cur_cmd = cmd_node->block_node;
	res = pipe_core_fd_control(p, cur_cmd, pipefd, mem);
	return (res);
}

int	pipe_fd_control_single_cmd(t_block_node *cur_cmd, t_mem **mem)
{
	struct termios	old_termios;
	int				res;
	bool			has_termios;

	has_termios = save_termios(&old_termios);
	if (!is_built_in(cur_cmd->cmd_arr))
		res = redir_files_validation(&cur_cmd->redirs_lst, mem, false);
	else
	{
		res = redir_files_validation(&cur_cmd->redirs_lst, mem, true);
		if (res != 0)
			return (res);
	}
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
	res = redir_files_validation(&cur_cmd->redirs_lst, mem, false);
	if (cur_cmd->input_lst != NULL)
		fd_input_redir(&cur_cmd->input_lst, mem);
	if (cur_cmd->output_lst != NULL)
		fd_output_redir(&cur_cmd->output_lst, mem);
	if (has_termios)
		restore_termios(&old_termios);
	return (res);
}
