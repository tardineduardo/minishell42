/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:39:05 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 21:00:34 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"
#include "../../include/expand.h"
#include "../../include/execution.h"

int	redir_files_validation(t_list **redir_lst, t_mem **mem, bool sngl_bi)
{
	t_list			*cur_node;
	t_redirs_node	*cur_redir;
	int				last_error;
	int				result;

	if (redir_lst == NULL || *redir_lst == NULL)
		return (-1);
	last_error = 0;
	cur_node = *redir_lst;
	while (cur_node)
	{
		cur_redir = cur_node->content;
		result = process_redirection(cur_redir, mem, sngl_bi);
		if (result != 0)
			last_error = result;
		cur_node = cur_node->next;
	}
	return (last_error);
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
