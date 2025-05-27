/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:28:25 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/27 13:31:55 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"
#include "../../include/expand.h"
#include "../../include/execution.h"
#include <termios.h>

int	redir_files_validation(t_list **redir_lst, t_mem **mem)
{
	int				fd;
	char			*cur_redir_expanded;
	t_list			*cur_node_redir;
	t_redirs_node	*cur_redir;

	if (redir_lst == NULL || *redir_lst == NULL)
		return (-1);
	cur_node_redir = *redir_lst;
	while (cur_node_redir)
	{
		cur_redir = cur_node_redir->content;
		cur_redir_expanded = ft_expand(&cur_redir->name, TOKEN, mem);
		if (cur_redir->type == IN_R || cur_redir->type == HDC_R)
		{
			if (access(cur_redir_expanded, F_OK) != 0)
			{
				ft_dprintf(2, "%s: No such file or directory\n", cur_redir_expanded);
				exit(1);
			}
			fd = open(cur_redir_expanded, O_RDONLY);
			if (fd == -1)
				exit(1);
			close(fd);
		}
		else if (cur_redir->type == OUT_R || cur_redir->type == APPD_R)
		{
			if (cur_redir->create == false)
			{
				fd = open(cur_redir_expanded, O_WRONLY | O_APPEND | O_CREAT, 0644);
				if (fd == -1)
				{
					ft_dprintf(2, "%s: Permission denied\n", cur_redir_expanded);
					exit(1);
				}
				close(fd);
			}
			else
			{
				fd = open(cur_redir_expanded, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (fd == -1)
				{
					ft_dprintf(2, "%s: Permission denied\n", cur_redir_expanded);
					return (1);
				}
				close(fd);
			}
		}
		cur_node_redir = cur_node_redir->next;
	}
	return (0);
}

int	file_input_handler(t_list **input_lst, t_mem **mem)
{
	int				fd;
	char			*cur_input_expanded;
	t_list			*cur_node_input;
	t_redirs_node	*cur_input;

	if (input_lst == NULL || *input_lst == NULL)
		return (-1);
	cur_node_input = *input_lst;
	while (cur_node_input)
	{
		cur_input = cur_node_input->content;
		cur_input_expanded = ft_expand(&cur_input->name, TOKEN, mem);
		if (access(cur_input_expanded, F_OK) != 0)
			exit(1);
		fd = open(cur_input_expanded, O_RDONLY);
		if (fd == -1)
			exit(1);
		if (cur_node_input->next == NULL)
			return (fd);
		close(fd);
		cur_node_input = cur_node_input->next;
	}
	return (0);
}

int	file_output_handler(t_list **output_lst, t_mem **mem)
{
	int				fd;
	char			*cur_output_expanded;
	t_list			*cur_node_output;
	t_redirs_node	*cur_output;

	if (output_lst == NULL || *output_lst == NULL)
		return (-1);
	cur_node_output = *output_lst;
	while (cur_node_output)
	{
		cur_output = cur_node_output->content;
		cur_output_expanded = ft_expand(&cur_output->name, TOKEN, mem);
		if (cur_output->type == OUT_R)
			fd = open(cur_output_expanded, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(cur_output_expanded, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			exit(1);
		if (cur_node_output->next == NULL)
			return (fd);
		close(fd);
		cur_node_output = cur_node_output->next;
	}
	return (0);
}

void	fd_input_redir(t_list **input_lst, t_mem **mem)
{
	int	input_redirect_fd;

	if (*input_lst != NULL)
	{
		input_redirect_fd = file_input_handler(input_lst, mem);
		if (input_redirect_fd >= 0)
		{
			dup2(input_redirect_fd, STDIN_FILENO);
			close(input_redirect_fd);
		}
	}
}

void	fd_output_redir(t_list **output_lst, t_mem **mem)
{
	int	output_redirect_fd;

	if (*output_lst != NULL)
	{
		output_redirect_fd = file_output_handler(output_lst, mem);
		if (output_redirect_fd >= 0)
		{
			dup2(output_redirect_fd, STDOUT_FILENO);
			close(output_redirect_fd);
		}
	}
}

bool save_termios(struct termios *saved)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, saved) == 0)
			return (true);
	}
	return (false);
}

bool restore_termios(struct termios *saved)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcsetattr(STDIN_FILENO, TCSANOW, saved) == 0)
			return (true);
	}
	return (false);
}

int	pipe_fd_control(t_pipe_data *pipe_data, t_block_node *cur_cmd, int pipefd[2], t_mem **mem)
{
	int res;
	struct termios old_termios;
	bool has_termios;

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
	int res;
	struct termios old_termios;
	bool has_termios;

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
	int res;
	struct termios old_termios;
	bool has_termios;

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