/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_ast_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:37:55 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/15 16:00:45 by luide-ca         ###   ########.fr       */
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
	t_ast_node **commands;
	int command_count;
}	t_pipeline_data;

typedef struct s_logical_data
{
	logical_op op;
	t_ast_node *left;
	t_ast_node *right;
}	t_logical_data;

typedef struct s_subshell_data
{
	t_ast_node *body;
}	t_subshell_data;

typedef struct s_command_data
{
	char **argv;
	int argc;
}	t_command_data;

typedef struct s_ast_node
{
	node_type type;
	union {
		t_command_data	cmd;
		t_pipeline_data pipeline;
		t_logical_data	logical;
		t_subshell_data subshell;
	};
}	t_ast_node;

// // Node types
// typedef enum {
// 	NODE_COMMAND,
// 	NODE_PIPELINE,
// 	NODE_LOGICAL,
// 	NODE_SUBSHELL
// } node_type;

// // Logical operators
// typedef enum {
// 	OP_NONE,
// 	OP_AND,
// 	OP_OR
// } logical_op;

// typedef struct s_ast_node
// {
// 	node_type			type;
// 	logical_op			op;
// 	t_list				*cmd_lst;
// 	struct s_ast_node	*left;
// 	struct s_ast_node	*right;
// 	struct s_ast_node	*body;
// }	t_ast_node;

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
t_ast_node *create_command_node(char **argv, int argc)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd.argv = argv;
	node->cmd.argc = argc;
	return node;
}

t_ast_node *create_pipeline_node(t_ast_node **commands, int count)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_PIPELINE;
	node->pipeline.commands = commands;
	node->pipeline.command_count = count;
	return node;
}

t_ast_node *create_logical_node(logical_op op, t_ast_node *left, t_ast_node *right)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_LOGICAL;
	node->logical.op = op;
	node->logical.left = left;
	node->logical.right = right;
	return node;
}

t_ast_node *create_group_node(t_ast_node *body)
{
	t_ast_node *node = malloc(sizeof(t_ast_node));
	node->type = NODE_SUBSHELL;
	node->subshell.body = body;
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
	while (cur && ft_strcmp(((t_tok_node *)cur->content)->tokstr, "||") == 0)
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
	while (cur && ft_strcmp(((t_tok_node *)cur->content)->tokstr, "&&") == 0)
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
	while (cur && ft_strcmp(((t_tok_node *)cur->content)->tokstr, "|") == 0)
	{
		cur = cur->next;
		right = parse_command_or_group(tokens);
		if (node->type == NODE_PIPELINE)
		{
			// Extend existing pipeline
			int new_count = node->data.pipeline.command_count + 1;
			t_ast_node **new_commands = realloc(node->data.pipeline.commands, new_count * sizeof(t_ast_node*));
			new_commands[new_count - 1] = right;
			node->data.pipeline.commands = new_commands;
			node->data.pipeline.command_count = new_count;
		} else {
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
	if (*tokens && strcmp((*tokens)->value, "(") == 0)
	{
		*tokens = (*tokens)->next; // Consume "("
		t_ast_node *node = parse_expression(tokens);
		if (!*tokens || strcmp((*tokens)->value, ")") != 0)
		{
			fprintf(stderr, "Error: Unclosed parenthesis\n");
			exit(1);
		}
		*tokens = (*tokens)->next; // Consume ")"
		return (create_group_node(node));
	}
	else
	{
		return (parse_command(tokens));
	}
}

// Parse a simple command
t_ast_node *parse_command(t_list **tokens) {
	char **argv = malloc(16 * sizeof(char*)); // Start with space for 16 args
	int argc = 0;
	int capacity = 16;
	
	while (*tokens && strcmp((*tokens)->value, "|") != 0 &&
					 strcmp((*tokens)->value, "&&") != 0 &&
					 strcmp((*tokens)->value, "||") != 0 &&
					 strcmp((*tokens)->value, ")") != 0) {
		if (argc >= capacity) {
			capacity *= 2;
			argv = realloc(argv, capacity * sizeof(char*));
		}
		argv[argc++] = strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	
	if (argc == 0) {
		fprintf(stderr, "Error: Empty command\n");
		exit(1);
	}
	
	argv = realloc(argv, argc * sizeof(char*)); // Trim to actual size
	return create_command_node(argv, argc);
}

// Free AST memory
void free_ast(t_ast_node *node) {
	if (!node) return;
	
	switch (node->type) {
		case NODE_COMMAND:
			for (int i = 0; i < node->data.command.argc; i++) {
				free(node->data.command.argv[i]);
			}
			free(node->data.command.argv);
			break;
		case NODE_PIPELINE:
			for (int i = 0; i < node->data.pipeline.command_count; i++) {
				free_ast(node->data.pipeline.commands[i]);
			}
			free(node->data.pipeline.commands);
			break;
		case NODE_LOGICAL:
			free_ast(node->data.logical.left);
			free_ast(node->data.logical.right);
			break;
		case NODE_GROUP:
			free_ast(node->data.group.body);
			break;
	}
	
	free(node);
}

// Print AST for debugging
void print_ast(t_ast_node *node, int depth) {
	if (!node) return;
	
	for (int i = 0; i < depth; i++) printf("  ");
	
	switch (node->type) {
		case NODE_COMMAND:
			printf("COMMAND:");
			for (int i = 0; i < node->data.command.argc; i++) {
				printf(" %s", node->data.command.argv[i]);
			}
			printf("\n");
			break;
		case NODE_PIPELINE:
			printf("PIPELINE:\n");
			for (int i = 0; i < node->data.pipeline.command_count; i++) {
				print_ast(node->data.pipeline.commands[i], depth + 1);
			}
			break;
		case NODE_LOGICAL:
			printf("LOGICAL %s:\n", 
				  node->data.logical.op == OP_AND ? "AND" : "OR");
			print_ast(node->data.logical.left, depth + 1);
			print_ast(node->data.logical.right, depth + 1);
			break;
		case NODE_SUBSHELL:
			printf("GROUP:\n");
			print_ast(node->data.group.body, depth + 1);
			break;
	}
}

// Free tokens
void free_tokens(Token *tokens) {
	while (tokens) {
		Token *next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
}

int main() {
	const char *input = "cmd1 && (cmd2 arg1 | cmd3) || cmd4 > out.txt";
	
	printf("Input: %s\n", input);
	
	// Tokenize
	Token *tokens = tokenize(input);
	
	// Parse
	ASTNode *ast = parse_expression(&tokens);
	
	if (tokens) {
		fprintf(stderr, "Error: Unexpected token '%s'\n", tokens->value);
		free_tokens(tokens);
		free_ast(ast);
		return 1;
	}
	
	// Print AST
	printf("\nAST:\n");
	print_ast(ast, 0);
	
	// Cleanup
	free_ast(ast);
	
	return 0;
}