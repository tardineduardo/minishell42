/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 08:21:19 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/25 12:49:49 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **ft_ms_env_arr(t_list **ms_env)
{
	t_env_node	*cur_ms_env_node;
	t_list		*current;
	char		**ms_env_cpy;
	char		*temp_key_value;
	char		*temp_key_sign;
	int			lst_size;
	int			i;

	if (!ms_env || !*ms_env)
    	return (NULL);
	lst_size = ft_lstsize(*ms_env);
	ms_env_cpy = (char **)calloc(sizeof(char *), (lst_size + 1));
	i = 0;
	current = *ms_env;
	while (current)
	{
		cur_ms_env_node = current->content;
		temp_key_sign = ft_strjoin(cur_ms_env_node->variable, "=");
		temp_key_value = ft_strjoin(temp_key_sign, cur_ms_env_node->value);
		free(temp_key_sign);
		ms_env_cpy[i] = temp_key_value;
		i++;
		current = current->next;
	}
	return (ms_env_cpy);
}
//--------------------------------error control
void	fork_error(pid_t pid)
{
	if (pid == -1)
	{
		//free(TODO: free everything);
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	pipe_error(int	pipe_response)
{
	if (pipe_response == -1)
	{
		//free(TODO: free everything);
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	fd_out_error(int fd_out)
{
	if (fd_out == -1)
	{
		//free(TODO: free everything);
		perror("fd out");
		exit(EXIT_FAILURE);
	}
}

void	fd_in_error(int fd_in)
{
	if (fd_in == -1)
	{
		//free(TODO: free everything);
		perror("fd in");
		exit(EXIT_FAILURE);
	}
}

void	execve_free_error(int execve)
{
	if (execve == -1)
	{
		//free(TODO: free everything);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

//-------------------------------------input and output handlers 
/*
	this function iterates through all input redirects, but 
	only reads from the last, exactly like it works on bash
*/
int	file_input_handler(t_list **input_lst)
{
	int				fd;
	t_list			*cur_node_input;
	t_input_node	*last_input;

	if (!input_lst || !*input_lst)
    	return (-1);
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
	fd_in_error(fd);
	return (fd);
}

int file_output_handler(t_list **output_lst)
{
	int				fd;
	t_list			*cur_node_output;
	t_output_node	*cur_output;

	if (!output_lst || !*output_lst)
    	return (-1);
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
			fd = open(cur_output->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (cur_output->create == false)
			fd = open(cur_output->name, O_WRONLY | O_APPEND | O_CREAT, 0644);
		/*
			only the last one, in the case of redirect output create, is necessary
			to pass the fd to caller and fill buffer in it if ok
		*/
		if (cur_node_output->next == NULL)
		{
			fd_out_error(fd);
			return (fd);
		}
		cur_node_output = cur_node_output->next;
	}
	return (0);
}

//------------------------------------------fds control
void	fd_pipe(int i, int num_cmds, int pipefd_0, int pipefd_1, int fd_in)
{
	if (i > 0) // If not the first command, read from previous pipe
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (i < num_cmds - 1) // If not the last command, write to next pipe
	{
		dup2(pipefd_1, STDOUT_FILENO);
		close(pipefd_1);
	}
	close(pipefd_0);  // Close unused read end
}

void	fd_input_redir(t_list **input_lst)
{
	int			input_redirect_fd;

	if (*input_lst != NULL) // if redirect input, read from the input source
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

	if (*output_lst != NULL) // if redirect output, write to the output source
	{
		output_redirect_fd = file_output_handler(output_lst);
		if (output_redirect_fd >= 0)
		{
			dup2(output_redirect_fd, STDOUT_FILENO);
			close(output_redirect_fd);
		}
	}
}

//-----------------------------------------------execution part: builtins and exeternal functions
/*
	When passing information to execve, we need to be sure
	that the path that we have is correct or we will incur
	in error. TODO create function to check and treat it
	(absolute, relative and so on).
*/
void	executor_external_cmd(char **cmd_arr, t_list **ms_env)
{
	char	**ms_env_cpy;

	if (!cmd_arr || !cmd_arr[0])
	{
		//free(TODO: free everything);
		perror("cmd_arr external cmd executor");
		exit(EXIT_FAILURE);
	}
	if (ms_env == NULL)
		execve_free_error(execve(cmd_arr[0], cmd_arr, NULL));
	else
	{
		ms_env_cpy = ft_ms_env_arr(ms_env);
		execve_free_error(execve(cmd_arr[0], cmd_arr, ms_env_cpy));
		/*TODO: here I need to free anything?*/
	}
}
bool	is_built_in(char **cmd_arr)
{
	if (ft_strncmp(cmd_arr[0], "pwd", 3) == 0 ||
			ft_strncmp(cmd_arr[0], "echo", 4) == 0 ||
			ft_strncmp(cmd_arr[0], "env", 3) == 0 ||
			ft_strncmp(cmd_arr[0], "cd", 2) == 0 ||
			ft_strncmp(cmd_arr[0], "export", 5) == 0 ||
			ft_strncmp(cmd_arr[0], "unset", 5) == 0 ||
			ft_strncmp(cmd_arr[0], "exit", 4) == 0)
		return (true);
	else
		return (false);
}
void	executor_built_in(char **cmd_arr, t_list **ms_env)
{
	if (!cmd_arr || !cmd_arr[0])
	{
		perror("cmd_arr built-in executor");
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(cmd_arr[0], "pwd", 3) == 0)
		ft_pwd(ms_env);
	else if (ft_strncmp(cmd_arr[0], "echo", 4) == 0)
		ft_echo(cmd_arr[0], false/* TODO update to cmd_arr[1]*/);
	else if (ft_strncmp(cmd_arr[0], "env", 3) == 0)
		ft_env(*ms_env);
	else if (ft_strncmp(cmd_arr[0], "cd", 2) == 0)
		ft_cd(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "export", 5) == 0)
		ft_export(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "unset", 5) == 0)
		ft_unset(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "exit", 4) == 0)
		ft_exit();
	perror("built-in executor");
	exit(EXIT_FAILURE);
}

void	command_executor(t_list **ms_env, t_command_node *cmd)
{
	char	**cmd_arr;

	cmd_arr = cmd->cmd_arr;
	if (is_built_in(cmd_arr[0]))
		executor_built_in(cmd_arr, ms_env);
	else
		executor_external_cmd(cmd_arr, ms_env);
}

//-------------------------------------------------brings everything together
int	multiple_pipes(t_list **cmd_lst, t_list **ms_env)
{
	t_list			*current_node;
	t_command_node	*cur_cmd;
	pid_t			cpid;
	int				pipefd[2];
	int				fd_in; // Initialize file descriptor for input
	int				num_cmds;
	int				i = 0;
	int				status;

	fd_in = 0;
	current_node = *cmd_lst;
	num_cmds = ft_lstsize(*cmd_lst);
	while (i < num_cmds && current_node)
	{
		if (i < num_cmds - 1)
			pipe_error(pipe(pipefd));
		cur_cmd = current_node->content;
		cpid = fork();
		pipe_error(cpid);
		if (cpid == 0) // Child process
		{
			fd_input_redir(&cur_cmd->input_lst);
			fd_pipe(i, num_cmds, pipefd[0], pipefd[1], fd_in);
			fd_output_redir(&cur_cmd->output_lst);
			command_executor(ms_env, cur_cmd);
		}
		if (i > 0) // Parent process
			close(fd_in);
		fd_in = pipefd[0]; // Store read end for next command
		close(pipefd[1]);  // Close write end
		current_node = current_node->next;
		i++;
	}
	while (waitpid(cpid, &status, 0) > 0); // Parent waits for all children
	return (WEXITSTATUS(status)); // TODO maybe this part isnt working properly, probably only the last status is returned
}
