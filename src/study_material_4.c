#include "./include/prototype.h"
/*
	remmember to organize the function in a better and ordely place
	and delete this one
*/
/*
	responsible to create child processes and their
	necessities
*/
int	*ft_pipe_fd(void)
{
	int	*pipefd;

	if (pipe(pipefd) == -1)
	{
		perror("pipefd");
		exit(EXIT_FAILURE);
	}
	return (pipefd);
}

pid_t	ft_fork_control(void)
{
	pid_t	cpid;

	cpid = fork();
	if (cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (cpid);
}

/*
	execution reponsabilities, first, cases targeting
	built in commands and next all the necessicities
	regarding external commands
*/	
//----------------------------------------------------------BUILT INS EXEC
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
void	exec_built_in(t_list **ms_env, t_cmd_node *cmd)
{
	char	**cmd_arr;

	cmd_arr = cmd->cmd_arr;
	if (!cmd_arr || !cmd_arr[0])
	{
		perror("cmd_arr built-in executor");
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(cmd_arr[0], "pwd", 3) == 0)
		ft_pwd(ms_env);
	else if (ft_strncmp(cmd_arr[0], "echo", 4) == 0)
		ft_echo(cmd_arr[0], cmd_arr[1]);
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

//---------------------------------------------------------------------EXTERNAL COMMANDS EXEC
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
//-------------------------------------------------UPDATE CMD_ARR WITH PATH OF BINARY
char **get_path_options(t_list **ms_env)
{
	t_env_node	*cur_env;
	t_list		*cur_node;
	char		**path_options;

	cur_node = *ms_env;
	while (cur_node)
	{
		cur_env = cur_node->content;
		if (ft_strcmp(cur_env->variable, "PATH") == 0)
		{
			path_options = ft_split_char(cur_env->value, ':');
			return (path_options);
		}
		cur_node = cur_node->next;
	}
	return (NULL);
}

char *get_right_path(char **path_options, char *cmd_binary)
{
	/*
		first strjoin the path_options[i] + cmd_binary
		pass it throught access if it exists and if it is executable
	*/
	char *test_path;
	int		i;
	int		num_options;

	i = 0;
	num_options = 0;
	while (path_options[num_options])
		num_options++;
	//num_options = ft_count_intem(path_options);
	while (i < num_options)
	{
		test_path = ft_strjoin(path_options[i], "/");
		test_path = ft_strjoin(test_path, cmd_binary);
		if (access(test_path, F_OK) == 0)
		{
			if (access(test_path, X_OK) == 0)
				return (test_path);
		}
		i++;
	}
	return (NULL);
}

char **update_cmd_arr(t_list **ms_env, char **cmd_arr)
{
	char **path_options;
	char *right_option;

	path_options = get_path_options(ms_env);
	right_option = get_right_path(path_options, cmd_arr[0]);
	cmd_arr[0] = ft_strdup(right_option);
	free(right_option);
	return (cmd_arr);
}

void	exec_external_cmd(t_list **ms_env, t_cmd_node *cmd)
{
	char	**cmd_arr;
	char	*cmd_path;

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

//-------------------------------------------------------------------------------EXECUTOR
void	ft_command_executor(t_list **ms_env, t_list **cmd)
{
	t_cmd_node	*cur_cmd;
	t_list		*cur_node;

	if (!cmd || !ms_env)
	{
		perror("cmd or ms_env: cmd executor");
		exit(EXIT_FAILURE);
	}
	cur_node = *cmd;
	cur_cmd = cur_node->content;
	if (is_built_in(cur_cmd->cmd_arr))
		exec_built_in(ms_env, cur_cmd);
	else
		exec_external_cmd(ms_env, cur_cmd);
	perror("cmd exec");
	exit(EXIT_FAILURE);
}

int	ft_prompt_execution(t_list **ms_env, t_list **cmd)
{
	pid_t	cpid;
	int		*pipefd;
	int		status;
	int		cmds_counter;

	if (!cmd || !ms_env)
	{
		perror("cmd or ms_env: execution");
		exit(EXIT_FAILURE);
	}
	cmds_counter = ft_lstsize(*cmd);
	pipefd = ft_pipe_fd();
	while (cmds_counter > 0)
	{
		cpid = ft_fork_control();
		if (cpid == 0)
			ft_command_executor(ms_env, cmd);
		cmds_counter--;
	}
	// make this part below a separate function, now only the last status is returned.
	while (waitpid(cpid, &status, 0) > 0);
	if (WIFEXITED(status))
    	return (WEXITSTATUS(status));
	return (-1);
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_list		*ms_env;
	t_cmd_node	*cmd;
	t_cmd_node	*cmd2;
	t_cmd_node	*cmd3;
	t_list		*cmd_lst;
	t_list		*cmd_node1;
	t_list		*cmd_node2;
	t_list		*cmd_node3;
	t_list		*input_lst;
	t_list		*input_lst_node1;
	t_list		*input_lst_node2;
	t_list		*output_lst;
	t_list		*output_lst_node1;
	t_list		*output_lst_node2;
	t_input		*input_node1;
	t_input		*input_node2;
	t_output	*output_node1;
	t_output	*output_node2;
	int			result;

	cmd = (t_cmd_node *)malloc(sizeof(t_cmd_node));
	cmd2 = (t_cmd_node *)malloc(sizeof(t_cmd_node));
	cmd3 = (t_cmd_node *)malloc(sizeof(t_cmd_node));
	if (!cmd || !cmd2 || !cmd3)
	{
		free(cmd);
		free(cmd2);
		free(cmd3);
	}
	cmd_node1 = (t_list *)malloc(sizeof(t_list));
	cmd_node2 = (t_list *)malloc(sizeof(t_list));
	cmd_node3 = (t_list *)malloc(sizeof(t_list));
	if (!cmd_node1 || !cmd_node2 || !cmd_node3)
	{
		free(cmd);
		free(cmd2);
		free(cmd3);
		free(cmd_node1);
		free(cmd_node2);
		free(cmd_node3);
		exit(EXIT_FAILURE);
	}
	input_lst_node1 = (t_list *)malloc(sizeof(t_list));
	input_lst_node2 = (t_list *)malloc(sizeof(t_list));
	output_lst_node1 = (t_list *)malloc(sizeof(t_list));
	output_lst_node2 = (t_list *)malloc(sizeof(t_list));
	if (!input_lst_node1 || !input_lst_node2 || !output_lst_node1 || !output_lst_node2)
	{
		free(input_lst_node1);
		free(input_lst_node2);
		free(output_lst_node1);
		free(output_lst_node2);
		free(cmd);
		free(cmd2);
		free(cmd3);
		free(cmd_node1);
		free(cmd_node2);
		free(cmd_node3);
		exit(EXIT_FAILURE);
	}
	input_node1 = (t_input *)malloc(sizeof(t_input));
	input_node2 = (t_input *)malloc(sizeof(t_input));
	output_node1 = (t_output *)malloc(sizeof(t_output));
	output_node2 = (t_output *)malloc(sizeof(t_output));
	if (!input_node1 || !input_node2 || !output_node1 || !output_node2)
	{
		free(input_node1);
		free(input_node2);
		free(output_node1);
		free(output_node2);
		free(input_lst_node1);
		free(input_lst_node2);
		free(output_lst_node1);
		free(output_lst_node2);
		free(cmd);
		free(cmd2);
		free(cmd3);
		free(cmd_node1);
		free(cmd_node2);
		free(cmd_node3);
		exit(EXIT_FAILURE);
	}

	cmd->cmd_arr = (char **)malloc(3 * sizeof(char *));
	cmd2->cmd_arr = (char **)malloc(2 * sizeof(char *));
	cmd3->cmd_arr = (char **)malloc(3 * sizeof(char *));
	if (!cmd->cmd_arr || !cmd2->cmd_arr || !cmd3->cmd_arr)
	{
		free(cmd->cmd_arr);
		free(cmd2->cmd_arr);
		free(cmd3->cmd_arr);
		free(input_node1);
		free(input_node2);
		free(output_node1);
		free(output_node2);
		free(input_lst_node1);
		free(input_lst_node2);
		free(output_lst_node1);
		free(output_lst_node2);
		free(cmd);
		free(cmd2);
		free(cmd3);
		free(cmd_node1);
		free(cmd_node2);
		free(cmd_node3);
		exit(EXIT_FAILURE);
	}
	// creating the input list. It is connected with the first node from cmds
	input_node1->name = "./auxiliary_files/input_test.txt";
	input_node1->fd = 0;
	input_node2->name = "./auxiliary_files/input_test2.txt";
	input_node2->fd = 0;
	// connecting t_input to content from t_list node and connecting t_list nodes
	input_lst_node1->content = (t_input *)input_node1;
	input_lst_node1->next = input_lst_node2;
	input_lst_node2->content = (t_input *)input_node2;
	input_lst_node2->next = NULL;
	input_lst = input_lst_node1;
	// creating the output list. It is connect with the last node from cmds
	output_node1->name = "output_test.txt";
	output_node1->fd = 0;
	output_node2->name = "output_test2.txt";
	output_node2->fd = 0;
	//connecting t_ouput to content from t_list and connecting t_list nodes
	output_lst_node1->content = (t_output *)output_node1;
	output_lst_node1->next = output_lst_node2;
	output_lst_node2->content = (t_output *)output_node2;
	output_lst_node2->next = NULL;
	output_lst = output_lst_node1;
	//initializing cmds
	cmd->cmd_arr[0] = "/usr/bin/cat";
	cmd->cmd_arr[1] = "./auxiliary_files/input_test.txt";
	cmd->cmd_arr[2] = NULL;
	cmd->input_lst = NULL;
	cmd->output_lst = NULL;
	cmd->heredoc_lst = NULL;

	cmd2->cmd_arr[0] = "/usr/bin/sort";
	cmd2->cmd_arr[1] = NULL;
	cmd2->input_lst = NULL;
	cmd2->output_lst = NULL;
	cmd->heredoc_lst = NULL;

	cmd3->cmd_arr[0] = "/usr/bin/grep";
	cmd3->cmd_arr[1] = "luide-ca";
	cmd3->cmd_arr[2] = NULL;
	cmd3->input_lst = NULL;
	cmd3->output_lst = output_lst;
	cmd->heredoc_lst = NULL;
	// connecting struct t_cmd_node to t_link nodes
	cmd_node1->content = (t_cmd_node *)cmd;
	cmd_node1->next = cmd_node2;
	cmd_node2->content = (t_cmd_node *)cmd2;
	cmd_node2->next = cmd_node3;
	cmd_node3->content = (t_cmd_node *)cmd3;
	cmd_node3->next = NULL;

	cmd_lst = cmd_node1;
	result = ft_prompt_execution(&ms_env, &cmd_lst);
	printf("result: %d\n", result);

	// Free memory
	free(input_node1);
	free(input_node2);
	free(output_node1);
	free(output_node2);
	free(input_lst_node1);
	free(input_lst_node2);
	free(output_lst_node1);
	free(output_lst_node2);
	free(cmd->cmd_arr);
	free(cmd2->cmd_arr);
	free(cmd3->cmd_arr);
	free(cmd);
	free(cmd2);
	free(cmd3);
	free(cmd_node1);
	free(cmd_node2);
	free(cmd_node3);

	return (0);
}
