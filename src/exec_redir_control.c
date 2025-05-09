/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_control.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:28:25 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/09 19:16:30 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../include/parsing.h"
#include "../include/expand.h"

int	redir_files_validation(t_list **redir_lst)
{
	int				fd;
	int				res;
	t_list			*cur_node_redir;
	t_redirs_node	*cur_redir;
	
	if (redir_lst == NULL || *redir_lst == NULL)
		return (-1);
	cur_node_redir = *redir_lst;
	while (cur_node_redir)
	{
		cur_redir = cur_node_redir->content;
		if (cur_redir->type == IN_R)
		{
			// if (access(cur_redir->name, F_OK) != 0)
			// {
			// 	ft_dprintf(2, "%s: No such file or directory", cur_redir->name);
			// 	exit(1);
			// }
			fd = open(cur_redir->name, O_RDONLY);
			if (fd == -1)
			{
				close(fd);
				ft_dprintf(2, "%s: Permission denied\n", cur_redir->name);
				res = 1;
			}
			close(fd);
		}
		else if (cur_redir->type == OUT_R)
		{
			if (cur_redir->create == false)
			{
				fd = open(cur_redir->name, O_WRONLY | O_APPEND | O_CREAT, 0644);
				if (fd == -1)
				{
					close(fd);
					ft_dprintf(2, "%s: Permission denied\n", cur_redir->name);
					res = 1;
				}
				close(fd);
			}
		}
		cur_node_redir = cur_node_redir->next;
	}
	return (res);
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
		{
			ft_dprintf(2, "%s: No such file or directory", cur_input_expanded);
			exit(1);
		}
		fd = open(cur_input_expanded, O_RDONLY);
		if (fd == -1)
		{
			close(fd);
			ft_dprintf(2, "%s: Permission denied\n", cur_input_expanded);
			exit(1);
		}
		if (cur_node_input->next == NULL)
			return (fd);
		close(fd);
		cur_node_input = cur_node_input->next;
	}
	return (0);              
}

int file_output_handler(t_list **output_lst, t_mem **mem)
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
		if (cur_output->create == true)
			fd = open(cur_output_expanded, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (cur_output->create == false)
			fd = open(cur_output_expanded, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
		{
			close(fd);
			ft_dprintf(2, "%s: Permission denied\n", cur_output_expanded);
			exit(1);
		}
		if (cur_node_output->next == NULL)
			return (fd);
		close(fd);
		cur_node_output = cur_node_output->next;
	}
	return (0);
}

int	fd_input_redir(t_list **input_lst, t_mem **mem)
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

int	fd_output_redir(t_list **output_lst, t_mem **mem)
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

int	pipe_fd_control(t_pipe_data *pipe_data, t_block_node *cur_cmd, int pipefd[2], t_mem **mem)
{
	int	res;

	res = redir_files_validation(&cur_cmd->redirs_lst);
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
	return (res);
}
