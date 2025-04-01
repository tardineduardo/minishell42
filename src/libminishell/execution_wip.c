#include "./include/prototype.h"

/*
	1. execute mode: one command
	2. execute mode: multiple commands
		a. define if it is first command or the last one
		b. in the middle pipes
	3. fds control
		a. understand the redirections
		b. understand if it is only write mode, only read mode, or write and read mode.
	4. make a waitpid control
*/

int	execute_cmd(t_list **ms_env, t_list **cmd)
{
	if (is_single_cmd())
		single_cmd();
	else
		multiple_cmds();
}

int	single_cmd(char **cmd_arr)
{
	
}

get_first_cmd()
{

}

get_last_cmd()
{
	
}

int multiple_cmd(t_list **ms_env, t_list **cmd)
{
	if (get_first_cmd(cmd))
		first_cmd();
	else if (get_last_cmd(cmd))
		last_cmd();
	else
		middle_cmds(cmd);
}