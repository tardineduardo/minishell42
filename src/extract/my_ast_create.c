/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_ast_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:37:55 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/18 19:05:19 by luide-ca         ###   ########.fr       */
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
	t_list	*cmds;					// t_list of t_cmd_node
	int 	command_count;
}	t_pipe_data;

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

typedef struct s_ast_node
{
	node_type 		type;
	t_cmd_node		*cmd;
	t_pipe_data *pipeline;
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
// void free_ast(t_ast_node *node);
// void print_ast(t_ast_node *node, int depth);

// Create a new command node
t_ast_node *create_command_node(t_cmd_node *cmd_data)
{
	t_ast_node	*node;
	t_cmd_node	*cmd;

	node = malloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	return (node);
}

t_ast_node *create_pipeline_node(t_cmd_node *cmd_data)
{
	t_ast_node	*node;
	t_list		*new_cmd;
	t_pipe_data *pipe_data;

	node = malloc(sizeof(t_pipe_data));
	node->type = NODE_PIPELINE;
	new_cmd = ft_lstnew(cmd_data);
	if (!pipe_data)
	{
		pipe_data = malloc(sizeof(t_pipe_data));
		ft_lstadd_back(pipe_data->cmds, new_cmd);
	}
	else
		ft_lstadd_back(pipe_data->cmds, cmd_data);
	node->pipeline = pipe_data->cmds;
	return (node);
}

t_ast_node *create_logical_node(logical_op op, t_ast_node *left, t_ast_node *right)
{
	t_ast_node		*node;
	t_logical_data	*logical;

	node = malloc(sizeof(t_logical_data));
	logical = malloc(sizeof(t_logical_data));
	node->type = NODE_LOGICAL;
	logical->op = op;
	logical->left = left;
	logical->right = right;
	node->logical = logical;
	return (node);
}

t_ast_node *create_group_node(t_ast_node *body)
{
	t_ast_node *node;

	node = malloc(sizeof(t_subshell_data));
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
	while (cur && ((t_org_tok *)cur->content)->oper == 4) // | oper
	{
		cur = cur->next;
		right = parse_command_or_group(tokens);
		if (node->type == NODE_PIPELINE)
		{
			cur = cur->next;
			((t_org_tok *)cur->content)->cmd_node = right->cmd;
			ft_lstadd_back(&node->pipeline->cmds, ((t_org_tok *)cur->content)->cmd_node);
		}
		else
		{
			// Create new pipeline
			t_ast_node **commands = malloc(2 * sizeof(t_ast_node*));
			commands[0] = node;
			commands[1] = right;
			node = create_pipeline_node(commands);
		}
	}
	return (node);
}

// Parse either a command or a group
t_ast_node *parse_command_or_group(t_list **tokens)
{
	t_list		*cur;

	cur = *tokens;
	if (cur && ((t_org_tok *)cur->content)->oper == 2) // ( oper
	{
		cur = cur->next; // Consume "("
		t_ast_node *node = parse_expression(tokens);
		if (!cur || ((t_org_tok *)cur->content)->oper == 3) // ) oper
		{
			fprintf(stderr, "Error: Unclosed parenthesis\n");
			exit(1);
		}
		cur = cur->next; // Consume ")"
		return (create_group_node(node));
	}
	else
		return (parse_command(tokens));
}

// Parse a simple command
t_ast_node *parse_command(t_list **tokens)
{
	t_list	*cur;

	cur = *tokens;
	if (((t_org_tok *)cur->content)->oper != -1)
		return create_command_node(((t_org_tok *)cur->content)->cmd_node);
}




















// Print AST for debugging
void print_ast(t_ast_node *node, int depth)
{
	if (!node) return;
	
	for (int i = 0; i < depth; i++) printf("  ");
	
	switch (node->type)
	{
		case NODE_COMMAND:
			printf("COMMAND:");
			printf(" %s", ((t_cmd_node *)node->cmd)->cmd_arr[0]);
			printf("\n");
			break;
		case NODE_PIPELINE:
			printf("PIPELINE:\n");
			while (node->pipeline->commands)
			{
				print_ast(((t_cmd_node *)node->pipeline->commands->content), depth + 1);
				node->pipeline->commands = node->pipeline->commands->next;
			}
			break;
		case NODE_LOGICAL:
			printf("LOGICAL %s:\n", 
				node->logical->op == OP_AND ? "AND" : "OR");
			print_ast(node->logical->left, depth + 1);
			print_ast(node->logical->right, depth + 1);
			break;
		case NODE_SUBSHELL:
			printf("GROUP:\n");
			print_ast(node->subshell->body, depth + 1);
			break;
	}
}
		
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