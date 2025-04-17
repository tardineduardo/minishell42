/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_ast_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:37:55 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/17 17:22:45 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_LOGICAL,
	NODE_SUBSHELL
}	node_type;

typedef enum e_logical_op
{
	OP_AND,
	OP_OR
}	logical_op;

typedef struct s_ast_node t_ast_node;

typedef struct s_pipeline_data
{
	t_list	*commands;
	int 	command_count;
}	t_pipeline_data;

typedef struct s_logical_data
{
	logical_op	op;
	t_ast_node	*left;
	t_ast_node	*right;
}	t_logical_data;

typedef struct s_subshell_data
{
	t_ast_node	*body;
}	t_subshell_data;

// typedef struct s_command_data
// {
// 	char **argv;
// 	int argc;
// }	t_command_data;

typedef struct s_cmd_data
{
	char	**cmd_arr;
	t_list	*input_lst;
	t_list	*output_lst;
	t_list	*heredoc_lst;
	t_list	*o_concat;
	int		err;
}			t_cmd_data;

typedef struct s_ast_node
{
	node_type 		type;
	t_cmd_data		*cmd;
	t_pipeline_data *pipeline;
	t_logical_data	*logical;
	t_subshell_data *subshell;
}	t_ast_node;

// Function prototypes
t_ast_node *parse_expression(t_list **tokens);
t_ast_node *parse_logical_or(t_list **tokens);
t_ast_node *parse_logical_and(t_list **tokens);
t_ast_node *parse_pipeline(t_list **tokens);
t_ast_node *parse_command_or_group(t_list **tokens);
t_ast_node *parse_command(t_list **tokens);
void free_ast(t_ast_node *node);
void print_ast(t_ast_node *node, int depth);

// Create a new command node
t_ast_node *create_command_node(t_cmd_data *cmd)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	return node;
}

t_ast_node *create_pipeline_node(t_list *commands, int count)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_PIPELINE;
	node->pipeline->commands = commands;
	return node;
}

t_ast_node *create_logical_node(logical_op op, t_ast_node *left, t_ast_node *right)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_LOGICAL;
	node->logical->op = op;
	node->logical->left = left;
	node->logical->right = right;
	return node;
}

t_ast_node *create_group_node(t_ast_node *body)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_SUBSHELL;
	node->subshell->body = body;
	return node;
}

// Parse an expression
t_ast_node *parse_expression(t_list **tokens)
{
	return parse_logical_or(tokens);
}

// Parse logical OR (||)
t_ast_node *parse_logical_or(t_list **tokens)
{
	t_ast_node	*node;
	t_ast_node 	*right;
	t_list		*cur;

	node = parse_logical_and(tokens);
	cur = *tokens;
	while (cur && ((t_org_tok *)cur->content)->oper == 1) // || operator 
	{
		cur = cur->next;
		right = parse_logical_and(tokens);
		node = create_logical_node(OP_OR, node, right);
	}
	return (node);
}

// Parse logical AND (&&)
t_ast_node *parse_logical_and(t_list **tokens)
{
	t_ast_node	*node;
	t_ast_node 	*right;
	t_list		*cur;

	node = parse_pipeline(tokens);
	cur = *tokens;
	while (cur && ((t_org_tok *)cur->content)->oper == 0) // && oper
	{
		cur = cur->next;
		right = parse_pipeline(tokens);
		node = create_logical_node(OP_AND, node, right);
	}
	return (node);
}

// Parse pipeline (|)
t_ast_node *parse_pipeline(t_list **tokens)
{
	t_ast_node	*node;
	t_ast_node 	*right;
	t_list		*cur;

	node = parse_command_or_group(tokens);
	cur = *tokens;
	while (cur && ((t_org_tok *)cur->content)->oper == 4)
	{
		cur = cur->next;
		right = parse_command_or_group(tokens);
		if (node->type == NODE_PIPELINE)
		{
			cur = cur->next;
			((t_org_tok *)cur->content)->cmd_node = right;
			ft_lstadd_back(node->pipeline->commands, ((t_org_tok *)cur->content)->cmd_node);
		}
		else
		{
			// Create new pipeline
			t_ast_node **commands = malloc(2 * sizeof(t_ast_node*));
			commands[0] = node;
			commands[1] = right;
			node = create_pipeline_node(commands, 2);
		}
	}
	return (node);
}

// Parse either a command or a group
t_ast_node *parse_command_or_group(t_list **tokens)
{
	t_list		*cur;

	cur = *tokens;
	if (cur && ((t_org_tok *)cur->content)->oper == 2)
	{
		cur = cur->next; // Consume "("
		t_ast_node *node = parse_expression(tokens);
		if (!cur || ((t_org_tok *)cur->content)->oper == 3)
		{
			fprintf(stderr, "Error: Unclosed parenthesis\n");
			exit(1);
		}
		cur = cur->next; // Consume ")"
		return (create_group_node(node));
	}
	else
	{
		return (parse_command(tokens));
	}
}

// Parse a simple command
t_ast_node *parse_command(t_list **tokens)
{
	t_list	*cur;

	cur = *tokens;
	if (((t_org_tok *)cur->content)->oper != -1)
		return create_command_node(((t_org_tok *)cur->content)->cmd_node);
}



















// Free AST memory
// void free_ast(t_ast_node *node) {
// 	if (!node) return;
	
// 	switch (node->type) {
// 		case NODE_COMMAND:
// 			for (int i = 0; i < node->data.command.argc; i++) {
// 				free(node->data.command.argv[i]);
// 			}
// 			free(node->data.command.argv);
// 			break;
// 		case NODE_PIPELINE:
// 			for (int i = 0; i < node->data.pipeline.command_count; i++) {
// 				free_ast(node->data.pipeline.commands[i]);
// 			}
// 			free(node->data.pipeline.commands);
// 			break;
// 		case NODE_LOGICAL:
// 			free_ast(node->data.logical.left);
// 			free_ast(node->data.logical.right);
// 			break;
// 		case NODE_GROUP:
// 			free_ast(node->data.group.body);
// 			break;
// 	}
	
// 	free(node);
// }

// Print AST for debugging
// void print_ast(t_ast_node *node, int depth) {
// 	if (!node) return;
	
// 	for (int i = 0; i < depth; i++) printf("  ");
	
// 	switch (node->type) {
// 		case NODE_COMMAND:
// 			printf("COMMAND:");
// 			for (int i = 0; i < node->data.command.argc; i++) {
// 				printf(" %s", node->data.command.argv[i]);
// 			}
// 			printf("\n");
// 			break;
// 		case NODE_PIPELINE:
// 			printf("PIPELINE:\n");
// 			for (int i = 0; i < node->data.pipeline.command_count; i++) {
// 				print_ast(node->data.pipeline.commands[i], depth + 1);
// 			}
// 			break;
// 		case NODE_LOGICAL:
// 			printf("LOGICAL %s:\n", 
// 				  node->data.logical.op == OP_AND ? "AND" : "OR");
// 			print_ast(node->data.logical.left, depth + 1);
// 			print_ast(node->data.logical.right, depth + 1);
// 			break;
// 		case NODE_SUBSHELL:
// 			printf("GROUP:\n");
// 			print_ast(node->data.group.body, depth + 1);
// 			break;
// 	}
// }

// Free tokens
// void free_tokens(Token *tokens) {
// 	while (tokens) {
// 		Token *next = tokens->next;
// 		free(tokens->value);
// 		free(tokens);
// 		tokens = next;
// 	}
// }

// int main() {
// 	const char *input = "cmd1 && (cmd2 arg1 | cmd3) || cmd4 > out.txt";
	
// 	printf("Input: %s\n", input);
	
// 	// Tokenize
// 	Token *tokens = tokenize(input);
	
// 	// Parse
// 	ASTNode *ast = parse_expression(&tokens);
	
// 	if (tokens) {
// 		fprintf(stderr, "Error: Unexpected token '%s'\n", tokens->value);
// 		free_tokens(tokens);
// 		free_ast(ast);
// 		return 1;
// 	}
	
// 	// Print AST
// 	printf("\nAST:\n");
// 	print_ast(ast, 0);
	
// 	// Cleanup
// 	free_ast(ast);
	
// 	return 0;
// }