#include "../include/prototype.h"

first_cmd(int fd[2], t_ast_tree *root, t_pipe_control *pipe_data)
{
	pid_t			cpid;

	pipe_data->fd_in = dup(fd[0]);
	if (pipe_data->fd_in == -1)
	{
		close(fd[0]);
		close(fd[1]);
		close(pipe_data->fd_in);
	}
	cpid = fork();
	if(cpid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		close(pipe_data->fd_in);
		exit(EXIT_FAILURE);
	}
	if (cpid == 0)
		only_write_cmd(root, fd, pipe_data);
}

inter_cmd(t_list **ms_env,t_cmd_node *cmd, t_pipe_control *pipe_data)
{

}

final_cmd(t_list **ms_env,t_cmd_node *cmd, t_pipe_control *pipe_data)
{

}

typedef struct s_pipe_control
{
	int	num_cmds;
	int	fd_in;
	int	fd_out;
}		t_pipe_control;