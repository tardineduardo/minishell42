#include "../include/prototype.h"

void	exec_external_cmd(t_list **ms_env, t_cmd_node *cmd)
{
	char	**cmd_arr;
	//char	*cmd_path;

	cmd_arr = cmd->cmd_arr;
	cmd_arr = update_cmd_arr(ms_env, cmd_arr);
	/*
		Studying about if is necessary to pass environ when calling execve, I understood
		that if I pass the binary with the right path (or the first arg from execve) it isnt
		necessary to pass environ (or the third param from execve) TODO: align this with edu
	*/
	if (execve(cmd_arr[0], cmd_arr, NULL) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}