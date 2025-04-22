/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_redir_control.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 09:27:21 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/22 14:54:43 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	file_input_handler(t_list **input_lst)
{
	int				fd;
	t_list			*cur_node_input;
	t_input_node	*last_input;

	if (input_lst == NULL || *input_lst == NULL)
		return (-1);
	cur_node_input = *input_lst;
	while (cur_node_input->next)
		cur_node_input = cur_node_input->next;
	last_input = cur_node_input->content;
	fd = open(last_input->name, O_RDONLY);
	if (fd == -1)
	{
		perror(last_input->name);
		exit(EXIT_FAILURE);
	}
	return (fd);              
}

int file_output_handler(t_list **output_lst)
{
	int				fd;
	t_list			*cur_node_output;
	t_output_node	*cur_output;

	if (output_lst == NULL || *output_lst == NULL)
		return (-1);
	cur_node_output = *output_lst;
	while (cur_node_output)
	{
		cur_output = cur_node_output->content;
		if (cur_output->create == true)
			fd = open(cur_output->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (cur_output->create == false)
			fd = open(cur_output->name, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
		{
			perror(cur_output->name);
			exit(EXIT_FAILURE);
		}
		if (cur_node_output->next == NULL)
			return (fd);
		close(fd);
		cur_node_output = cur_node_output->next;
	}
	return (0);
}

void	fd_input_redir(t_list **input_lst)
{
	int			input_redirect_fd;

	if (*input_lst != NULL)
	{
		input_redirect_fd = file_input_handler(input_lst);
		if (input_redirect_fd >= 0)
		{
			dup2(input_redirect_fd, STDIN_FILENO);
			close(input_redirect_fd);
		}
	}
}

void	fd_output_redir(t_list **output_lst)
{
	int			output_redirect_fd;

	if (*output_lst != NULL)
	{
		output_redirect_fd = file_output_handler(output_lst);
		if (output_redirect_fd >= 0)
		{
			dup2(output_redirect_fd, STDOUT_FILENO);
			close(output_redirect_fd);
		}
	}
}

void	pipe_fd_control(t_pipe_data *pipe_data, t_cmd_node *cur_cmd, int pipefd[2])
{
	// if (cur_cmd->input_lst != NULL)  
	// 	fd_input_redir(&cur_cmd->input_lst);
	(void)cur_cmd;
	if (pipe_data->i > 0)
	{
		dup2(pipe_data->prev_fd, STDIN_FILENO);
		close(pipe_data->prev_fd);
	}
	// if (cur_cmd->output_lst != NULL) 
	// 	fd_output_redir(&cur_cmd->output_lst);
	if (pipe_data->i < pipe_data->num_cmds - 1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
}