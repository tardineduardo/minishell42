#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/minishell.h"  // Inclua o header com as structs e prototypes

// ========== Funções auxiliares ==========

t_cmd_node *mock_cmd(char **args)
{
	t_cmd_node *cmd = malloc(sizeof(t_cmd_node));
	cmd->cmd_arr = args;
	cmd->input_lst = NULL;
	cmd->output_lst = NULL;
	cmd->err = 0;
	return cmd;
}

t_ast_node *mock_command_node(char **args)
{
	return create_command_node(mock_cmd(args));
}

t_list *mock_list_from_nodes(t_ast_node **nodes, int count)
{
	t_list *head = NULL;
	for (int i = 0; i < count; i++)
		ft_lstadd_back(&head, ft_lstnew(nodes[i]));
	return head;
}

// ========== Testes ==========

void test_simple_command()
{
	printf("==== Teste: Simple Command ====\n");
	char *args[] = {"ls", "-l", NULL};
	t_ast_node *root = mock_command_node(args);
	print_ast(root, 0);
}

void test_pipeline()
{
	printf("==== Teste: Pipeline ====\n");
	char *args1[] = {"ls", "-l", NULL};
	char *args2[] = {"sort", NULL};

	t_ast_node *cmd1 = mock_command_node(args1);
	t_ast_node *cmd2 = mock_command_node(args2);

	t_ast_node **cmds = malloc(2 * sizeof(t_ast_node *));
	cmds[0] = cmd1;
	cmds[1] = cmd2;

	t_list *cmd_list = mock_list_from_nodes(cmds, 2);
	t_ast_node *pipeline = create_pipeline_node(cmd_list);
	print_ast(pipeline, 0);
}

void test_logical()
{
	printf("==== Teste: Logical AND ====\n");
	char *args1[] = {"ls", NULL};
	char *args2[] = {"echo", "done", NULL};

	t_ast_node *cmd1 = mock_command_node(args1);
	t_ast_node *cmd2 = mock_command_node(args2);

	t_ast_node *logical = create_logical_node(OP_AND, cmd1, cmd2);
	print_ast(logical, 0);
}

void test_subshell()
{
	printf("==== Teste: Subshell ====\n");
	char *args1[] = {"echo", "hi", NULL};
	char *args2[] = {"wc", "-l", NULL};

	t_ast_node *cmd1 = mock_command_node(args1);
	t_ast_node *cmd2 = mock_command_node(args2);

	t_ast_node **cmds = malloc(2 * sizeof(t_ast_node *));
	cmds[0] = cmd1;
	cmds[1] = cmd2;

	t_list *cmd_list = mock_list_from_nodes(cmds, 2);
	t_ast_node *pipe = create_pipeline_node(cmd_list);

	t_ast_node *group = create_group_node(pipe);
	print_ast(group, 0);
}

// ========== Main ==========

int main(void)
{
	test_simple_command();
	printf("\n");

	test_pipeline();
	printf("\n");

	test_logical();
	printf("\n");

	test_subshell();
	return 0;
}
