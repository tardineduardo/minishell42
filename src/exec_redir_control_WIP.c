

#include "../include/minishell.h"
#include "../include/parsing.h"
#include "../include/expand.h"

typedef struct s_redir_control
{
	int	fd_in;
	int fd_out;
	int	err;
}	t_redir_control;

t_redir_control	*redir_files_validation(t_list **redir_lst, t_mem **mem)
{
	int				fd_in;
	int				fd_out;
	char			*cur_redir_expanded;
	t_list			*cur_node_redir;
	t_redirs_node	*cur_redir;
	t_redir_control	*redir_control;
	
	if (redir_lst == NULL || *redir_lst == NULL)
		return (NULL);
	redir_control = malloc(sizeof(t_redir_control));
	if (!redir_control)
		return (NULL);
	redir_control->fd_in = -2;
	redir_control->fd_out = -2;
	redir_control->err = 0;
	cur_node_redir = *redir_lst;
	while (cur_node_redir)
	{
		cur_redir = cur_node_redir->content;
		cur_redir_expanded = ft_expand(&cur_redir->name, TOKEN, mem);
		if (cur_redir->type == IN_R || cur_redir->type == HDC_R)
		{
			if (access(cur_redir_expanded, F_OK) != 0)
			{
				ft_dprintf(2, "%s: No such file or directory", cur_redir_expanded);
				redir_control->err = 1;
				return (redir_control);
			}
			fd_in = open(cur_redir_expanded, O_RDONLY);
			if (fd_in == -1)
			{
				ft_dprintf(2, "%s: Permission denied\n", cur_redir_expanded);
				redir_control->err = 1;
				return (redir_control);
			}
			if (fd_in != redir_control->fd_in)
				redir_control->fd_in = fd_in;
			if (redir_control->fd_in >= 0 && fd_in != redir_control->fd_in)
				close(redir_control->fd_in);
		}
		else if (cur_redir->type == OUT_R || cur_redir->type == APPD_R)
		{
			if (cur_redir->create == true)
				fd_out = open(cur_redir_expanded, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			else if (cur_redir->create == false)
				fd_out = open(cur_redir_expanded, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (fd_out == -1)
			{
				ft_dprintf(2, "%s: Permission denied\n", cur_redir_expanded);
				redir_control->err = 1;
				return (redir_control);
			}
			if (fd_out != redir_control->fd_out)
				redir_control->fd_out = fd_out;
			if (redir_control->fd_out >= 0 && fd_out != redir_control->fd_out)
				close(redir_control->fd_out);
		}
		cur_node_redir = cur_node_redir->next;
	}
	free(cur_redir_expanded);
	return (redir_control);
}

// int	file_input_handler(t_list **input_lst, t_mem **mem)
// {
// 	int				fd;
// 	char			*cur_input_expanded;
// 	t_list			*cur_node_input;
// 	t_redirs_node	*cur_input;

// 	if (input_lst == NULL || *input_lst == NULL)
// 		return (-1);
// 	cur_node_input = *input_lst;
// 	while (cur_node_input)
// 	{
// 		cur_input = cur_node_input->content;
// 		cur_input_expanded = ft_expand(&cur_input->name, TOKEN, mem);
// 		if (access(cur_input_expanded, F_OK) != 0)
// 		{
// 			ft_dprintf(2, "%s: No such file or directory", cur_input_expanded);
// 			exit(1);
// 		}
// 		fd = open(cur_input_expanded, O_RDONLY);
// 		if (fd == -1)
// 		{
// 			close(fd);
// 			ft_dprintf(2, "%s: Permission denied\n", cur_input_expanded);
// 			exit(1);
// 		}
// 		if (cur_node_input->next == NULL)
// 			return (fd);
// 		close(fd);
// 		cur_node_input = cur_node_input->next;
// 	}
// 	return (0);              
// }

// int file_output_handler(t_list **output_lst, t_mem **mem)
// {
// 	int				fd;
// 	char			*cur_output_expanded;
// 	t_list			*cur_node_output;
// 	t_redirs_node	*cur_output;

// 	if (output_lst == NULL || *output_lst == NULL)
// 		return (-1);
// 	cur_node_output = *output_lst;
// 	while (cur_node_output)
// 	{
// 		cur_output = cur_node_output->content;
// 		cur_output_expanded = ft_expand(&cur_output->name, TOKEN, mem);
// 		if (cur_output->create == true)
// 			fd = open(cur_output_expanded, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 		else if (cur_output->create == false)
// 			fd = open(cur_output_expanded, O_WRONLY | O_APPEND | O_CREAT, 0644);
// 		if (fd == -1)
// 		{
// 			close(fd);
// 			ft_dprintf(2, "%s: Permission denied\n", cur_output_expanded);
// 			exit(1);
// 		}
// 		if (cur_node_output->next == NULL)
// 			return (fd);
// 		close(fd);
// 		cur_node_output = cur_node_output->next;
// 	}
// 	return (0);
// }

void	fd_input_redir(int	fd_in)
{
	if (fd_in >= 0)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
}

void	fd_output_redir(int	fd_out)
{
	if (fd_out >= 0)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

int	pipe_fd_control_pipe(t_pipe_data *pipe_data, t_block_node *cur_cmd, int pipefd[2], t_mem **mem)
{
	t_redir_control	*res;

	res = redir_files_validation(&cur_cmd->redirs_lst, mem);
	if (res->fd_in >= 0) 
		fd_input_redir(res->fd_in);
	else if (pipe_data->i > 0)
	{
		dup2(pipe_data->prev_fd, STDIN_FILENO);
		close(pipe_data->prev_fd);
	}
	if (res->fd_out >= 0)
		fd_output_redir(res->fd_out);
	else if (pipe_data->i < pipe_data->num_cmds - 1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (res->err != 0)
		exit(res->err);
	return (0);
}

int	pipe_fd_control_single_cmd(t_block_node *cur_cmd, t_mem **mem)
{
	t_redir_control	*res;

	res = redir_files_validation(&cur_cmd->redirs_lst, mem);
	if (res->fd_in >= 0) 
		fd_input_redir(res->fd_in);
	if (res->fd_out >= 0)
		fd_output_redir(res->fd_out);
	if (res->err != 0)
		exit(res->err);
	return (0);
}