#include "./include/prototype.h"

int excution_prom(t_list **content)
{
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
	t_input_node	*input_node1;
	t_input_node	*input_node2;
	t_output_node	*output_node1;
	t_output_node	*output_node2;
	//t_env_node		*ms_env;
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
	input_node1 = (t_input_node *)malloc(sizeof(t_input_node));
	input_node2 = (t_input_node *)malloc(sizeof(t_input_node));
	output_node1 = (t_output_node *)malloc(sizeof(t_output_node));
	output_node2 = (t_output_node *)malloc(sizeof(t_output_node));
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

	cmd->cmd_arr = (char **)malloc(2 * sizeof(char *));
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
	input_node1->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/input_test.txt";
	input_node2->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/input_test2.txt";
	// connecting t_input to content from t_list node and connecting t_list nodes
	input_lst_node1->content = (t_input_node *)input_node1;
	input_lst_node1->next = input_lst_node2;
	input_lst_node2->content = (t_input_node *)input_node2;
	input_lst_node2->next = NULL;
	input_lst = input_lst_node1;
	// creating the output list. It is connect with the last node from cmds
	output_node1->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/output_test.txt";
	output_node1->create = true;
	output_node2->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/output_test2.txt";
	output_node2->create = true;
	//connecting t_ouput to content from t_list and connecting t_list nodes
	output_lst_node1->content = (t_output_node *)output_node1;
	output_lst_node1->next = output_lst_node2;
	output_lst_node2->content = (t_output_node *)output_node2;
	output_lst_node2->next = NULL;
	output_lst = output_lst_node1;
	//initializing cmds
	cmd->cmd_arr[0] = "cat";
	//cmd->cmd_arr[1] = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/input_test.txt";
	cmd->cmd_arr[1] = NULL;
	cmd->input_lst = input_lst;
	cmd->output_lst = NULL;
	cmd->heredoc_lst = NULL;

	cmd2->cmd_arr[0] = "/usr/bin/sort";
	cmd2->cmd_arr[1] = NULL;
	cmd2->input_lst = NULL;
	cmd2->output_lst = NULL;
	cmd->heredoc_lst = NULL;

	cmd3->cmd_arr[0] = "grep";
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
	result = exec_pipe(content, &cmd_lst);
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

// int excution_prom(t_list **content)
// {
// 	t_cmd_node	*cmd;
// 	t_list		*cmd_lst;
// 	t_list		*cmd_node1;
// 	t_list		*input_lst;
// 	t_list		*input_lst_node1;
// 	t_list		*input_lst_node2;
// 	t_list		*output_lst;
// 	t_list		*output_lst_node1;
// 	t_list		*output_lst_node2;
// 	t_input_node	*input_node1;
// 	t_input_node	*input_node2;
// 	t_output_node	*output_node1;
// 	t_output_node	*output_node2;
// 	//t_env_node		*ms_env;
// 	int			result;

// 	cmd = (t_cmd_node *)malloc(sizeof(t_cmd_node));

// 	if (!cmd)
// 	{
// 		free(cmd);

// 	}
// 	cmd_node1 = (t_list *)malloc(sizeof(t_list));

// 	if (!cmd_node1)
// 	{
// 		free(cmd);
// 		free(cmd_node1);
// 		exit(EXIT_FAILURE);
// 	}
// 	input_lst_node1 = (t_list *)malloc(sizeof(t_list));
// 	input_lst_node2 = (t_list *)malloc(sizeof(t_list));
// 	output_lst_node1 = (t_list *)malloc(sizeof(t_list));
// 	output_lst_node2 = (t_list *)malloc(sizeof(t_list));
// 	if (!input_lst_node1 || !input_lst_node2 || !output_lst_node1 || !output_lst_node2)
// 	{
// 		free(input_lst_node1);
// 		free(input_lst_node2);
// 		free(output_lst_node1);
// 		free(output_lst_node2);
// 		free(cmd);
// 		free(cmd_node1);
// 		exit(EXIT_FAILURE);
// 	}
// 	input_node1 = (t_input_node *)malloc(sizeof(t_input_node));
// 	input_node2 = (t_input_node *)malloc(sizeof(t_input_node));
// 	output_node1 = (t_output_node *)malloc(sizeof(t_output_node));
// 	output_node2 = (t_output_node *)malloc(sizeof(t_output_node));
// 	if (!input_node1 || !input_node2 || !output_node1 || !output_node2)
// 	{
// 		free(input_node1);
// 		free(input_node2);
// 		free(output_node1);
// 		free(output_node2);
// 		free(input_lst_node1);
// 		free(input_lst_node2);
// 		free(output_lst_node1);
// 		free(output_lst_node2);
// 		free(cmd);
// 		free(cmd_node1);
// 		exit(EXIT_FAILURE);
// 	}

// 	cmd->cmd_arr = (char **)malloc(2 * sizeof(char *));
// 	if (!cmd->cmd_arr)
// 	{
// 		free(cmd->cmd_arr);
// 		free(input_node1);
// 		free(input_node2);
// 		free(output_node1);
// 		free(output_node2);
// 		free(input_lst_node1);
// 		free(input_lst_node2);
// 		free(output_lst_node1);
// 		free(output_lst_node2);
// 		free(cmd);
// 		free(cmd_node1);
// 		exit(EXIT_FAILURE);
// 	}
// 	// creating the input list. It is connected with the first node from cmds
// 	input_node1->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/input_test.txt";
// 	input_node2->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/input_test2.txt";
// 	// connecting t_input to content from t_list node and connecting t_list nodes
// 	input_lst_node1->content = (t_input_node *)input_node1;
// 	input_lst_node1->next = input_lst_node2;
// 	input_lst_node2->content = (t_input_node *)input_node2;
// 	input_lst_node2->next = NULL;
// 	input_lst = input_lst_node1;
// 	// creating the output list. It is connect with the last node from cmds
// 	output_node1->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/output_test.txt";
// 	output_node1->create = true;
// 	output_node2->name = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/output_test2.txt";
// 	output_node2->create = true;
// 	//connecting t_ouput to content from t_list and connecting t_list nodes
// 	output_lst_node1->content = (t_output_node *)output_node1;
// 	output_lst_node1->next = output_lst_node2;
// 	output_lst_node2->content = (t_output_node *)output_node2;
// 	output_lst_node2->next = NULL;
// 	output_lst = output_lst_node1;
// 	//initializing cmds
// 	cmd->cmd_arr[0] = "pwd";
// 	//cmd->cmd_arr[1] = "/nfs/homes/luide-ca/common-core-github/minishell-github-edu/src/auxiliary_files/input_test.txt";
// 	cmd->cmd_arr[1] = NULL;
// 	cmd->input_lst = input_lst;
// 	cmd->output_lst = output_lst;
// 	cmd->heredoc_lst = NULL;

// 	// connecting struct t_cmd_node to t_link nodes
// 	cmd_node1->content = (t_cmd_node *)cmd;
// 	cmd_node1->next = NULL;

// 	cmd_lst = cmd_node1;
// 	result = exec_pipeline(content, &cmd_lst);
// 	printf("result: %d\n", result);

// 	// Free memory
// 	free(input_node1);
// 	free(input_node2);
// 	free(output_node1);
// 	free(output_node2);
// 	free(input_lst_node1);
// 	free(input_lst_node2);
// 	free(output_lst_node1);
// 	free(output_lst_node2);
// 	free(cmd->cmd_arr);
// 	free(cmd);
// 	free(cmd_node1);

// 	return (0);
// }