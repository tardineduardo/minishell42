#include "./include/prototype.h"

void	exec_pipe_command(t_pipe_control *pipe_data, int pipefd[2], t_list **ms_env, t_cmd_node *cur_cmd)
{
	char	**cmd_arr;
	pid_t	cpid;

	cmd_arr = cur_cmd->cmd_arr;
	if (!cur_cmd || !ms_env)
	{
		perror("cmd or ms_env: cmd executor");
		exit(EXIT_FAILURE);
	}
	cpid = ft_fork_control();
	if (cpid == 0)
	{
		pipe_fd_control(pipe_data, cur_cmd, pipefd);
		if (is_built_in(cmd_arr))
			exec_built_in(ms_env, cmd_arr);
		else
			exec_external_cmd(ms_env, cur_cmd);
	}
	if (pipe_data->i > 0) // Close previous read end
		close(pipe_data->fd_next);
	pipe_data->fd_next = pipefd[0]; // Update for next iteration
	if (pipe_data->i < pipe_data->num_cmds - 1)
		close(pipefd[1]); // Close write end after use
}

int	exec_pipe(t_list **ms_env, t_list **cmd)
{
	int				*pipefd;
	int				status;
	t_cmd_node		*cur_cmd;
	t_list			*cur_node;
	t_pipe_control	*pipe_data;

	pipe_data = (t_pipe_control *)malloc(sizeof(t_pipe_control));
	if (!pipe_data)
		return (1);
	pipe_data->fd_next = 0;
	if (!cmd || !ms_env)
	{
		perror("cmd or ms_env: exec pipeline");
		exit(EXIT_FAILURE);
	}
	pipe_data->num_cmds = ft_lstsize(*cmd);
	cur_node = *cmd;
	pipe_data->i = 0;
	while (pipe_data->i < pipe_data->num_cmds)
	{
		cur_cmd = cur_node->content;
		pipefd = ft_pipe_run();
		exec_pipe_command(pipe_data, pipefd, ms_env, cur_cmd);
		cur_node = cur_node->next;
		pipe_data->i++;
	}
	while (waitpid(-1, &status, 0) > 0)
	if (WIFEXITED(status))
    	return (WEXITSTATUS(status));
	return (-1);
}
