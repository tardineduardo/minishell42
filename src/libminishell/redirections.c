#include "./include/prototype.h"

void	pipefd_control(int i, int num_cmds, int pipefd_0, int pipefd_1, int fd_in)
{
	if (i > 0) // If not the first command, read from previous pipe
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(EXIT_FAILURE);
		}
		close(fd_in);
	}
	if (i < num_cmds - 1) // If not the last command, write to next pipe
	{
		dup2(pipefd_1, STDOUT_FILENO);
		close(pipefd_1);
	}
	close(pipefd_0);  // Close unused read end
}

int	file_input_handler(t_list **input_lst)
{
	int		fd;
	t_list	*cur_node_input;
	t_input_node	*last_input;

	cur_node_input = *input_lst;
	while(cur_node_input->next)
		cur_node_input = cur_node_input->next;
	last_input = cur_node_input->content;
	if (access(last_input->name, F_OK) != 0)
	{
		perror("access input");
		exit(EXIT_FAILURE);
	}
	fd = open(last_input->name, O_RDONLY);
	if (fd == -1)
	{
		perror("fd in");
		exit(EXIT_FAILURE);
	}
	return (fd);                 
}

int file_output_handler(t_list **output_lst)
{
	int			fd;
	t_list		*cur_node_output;
	t_output_node	*cur_output;

	cur_node_output = *output_lst;
	while (cur_node_output)
	{
		cur_output = cur_node_output->content;
		/*
			TODO make sure that this part is ok: if create is false, all the files 
			need to be created? I mean if the file doesnt exist, I create it. And 
			understand of multiples append riderections behavior
		*/
		if (cur_output->create == true)
		{
			fd = open(cur_output->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("fd out");
				exit(EXIT_FAILURE);
			}
		}
		else if (cur_output->create == false)
		{
			fd = open(cur_output->name, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (fd == -1)
			{
				perror("fd out");
				exit(EXIT_FAILURE);
			}
		}
		/*
			only the last one, in the case of redirect output create, is necessary
			to pass the fd to caller and fill buffer in it if ok
		*/
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

	if (*input_lst != NULL) // if redirect input, read from the input source
	{
		input_redirect_fd = file_input_handler(input_lst);
		if (input_redirect_fd > 0)
		{
			dup2(input_redirect_fd, STDIN_FILENO);
			close(input_redirect_fd);
		}
	}
}

void	fd_output_redir(t_list **output_lst)
{
	int			output_redirect_fd;

	if (*output_lst != NULL) // if redirect output, write to the output source
	{
		output_redirect_fd = file_output_handler(output_lst);
		if (output_redirect_fd > 0)
		{
			dup2(output_redirect_fd, STDOUT_FILENO);
			close(output_redirect_fd);
		}
	}
}
