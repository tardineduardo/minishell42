/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_ast_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:01:22 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/19 17:01:25 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Function prototypes
t_ast_node *parse_expression(t_list **tokens);
t_ast_node *parse_logical_or(t_list **tokens);
t_ast_node *parse_logical_and(t_list **tokens);
t_ast_node *parse_pipeline(t_list **tokens);
t_ast_node *parse_command_or_group(t_list **tokens);
t_ast_node *parse_command(t_list **tokens);
// void free_ast(t_ast_node *node);

// Create a new command node
t_ast_node *create_command_node(t_cmd_node *cmd_data)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->cmd = cmd_data;
	return (node);
}

t_ast_node *create_pipeline_node(t_list *cmds_lst)
{
    t_ast_node *node = malloc(sizeof(t_ast_node));
    t_pipe_info *pipe_data = malloc(sizeof(t_pipe_info));
    if (!node || !pipe_data) {
        if (node) free(node);
        if (pipe_data) free(pipe_data);
        return NULL;
    }

    node->type = NODE_PIPELINE;
    pipe_data->cmds = cmds_lst;
    pipe_data->cmd_count = 0;
    
    // Count commands
    t_list *current = cmds_lst;
    while (current) {
        pipe_data->cmd_count++;
        current = current->next;
    }
    
    node->pipeline = pipe_data;
    node->cmd = NULL;
    node->logical = NULL;
    node->subshell = NULL;
    return node;
}

t_ast_node *create_logical_node(logical_op op, t_ast_node *left, t_ast_node *right)
{
	t_ast_node		*node;
	t_logical_data	*logical;

	node = malloc(sizeof(t_ast_node));
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

	node = malloc(sizeof(t_ast_node));
	node->subshell = malloc(sizeof(t_subshell_data));
	node->type = NODE_SUBSHELL;
	node->subshell->body = body;
	return node;
}
//----------------------------------------PARSING NODES------------------------------------
// Parse an expression
t_ast_node *parse_expression(t_list **tokens)
{
	return parse_logical_or(tokens);
}

// Parse logical OR (||)
t_ast_node *parse_logical_or(t_list **tokens)
{
    t_ast_node *node;
    t_ast_node *right;

    node = parse_logical_and(tokens);
    if (!node) 
        return (NULL);
    while (*tokens && ((t_org_tok *)(*tokens)->content)->oper == 1) // || operator
    {
        *tokens = (*tokens)->next;
        right = parse_logical_and(tokens);
        if (!right)
            return (NULL);
        node = create_logical_node(OP_OR, node, right);
    }
    return node;
}

// Parse logical AND (&&)
t_ast_node *parse_logical_and(t_list **tokens)
{
    t_ast_node *node;
    t_ast_node *right;

    node = parse_pipeline(tokens);
    if (!node)
        return (NULL);
    while (*tokens && ((t_org_tok *)(*tokens)->content)->oper == 0) // && operator
    {
        *tokens = (*tokens)->next;
        right = parse_pipeline(tokens);
        if (!right)
            return( NULL);
        node = create_logical_node(OP_AND, node, right);
    }
    return (node);
}

// Parse pipeline (|)
t_ast_node *parse_pipeline(t_list **tokens)
{
    t_ast_node *node;
    t_list *cmds_lst;
    t_ast_node *right;
    t_list *new_node;
    t_ast_node *pipeline_node;

    node = parse_command_or_group(tokens);
    if (!node)
        return (NULL);
    // If we don't have a pipe, return the single command
    if (!*tokens || ((t_org_tok *)(*tokens)->content)->oper != 4)
        return (node);
    // We have a pipeline - create a list of commands
    cmds_lst = ft_lstnew(node->cmd);
    if (!cmds_lst)
        return (NULL);
    while (*tokens && ((t_org_tok *)(*tokens)->content)->oper == 4)
    {
        *tokens = (*tokens)->next; // Consume '|'
        right = parse_command_or_group(tokens);
        if (!right || right->type != NODE_COMMAND)
        {
            fprintf(stderr, "Invalid pipeline structure\n");
            exit(1);
        }
        new_node = ft_lstnew(right->cmd);
        if (!new_node)
        {
            // Free the list we created
            while (cmds_lst)
            {
                t_list *next = cmds_lst->next;
                free(cmds_lst);
                cmds_lst = next;
            }
            return (NULL);
        }
        ft_lstadd_back(&cmds_lst, new_node);
        // Free the right node (we only need its cmd)
        free(right);
    }
    // Create the pipeline node with all commands
    pipeline_node = create_pipeline_node(cmds_lst);
    if (!pipeline_node)
    {
        // Free the list we created
        while (cmds_lst)
        {
            t_list *next = cmds_lst->next;
            free(cmds_lst);
            cmds_lst = next;
        }
        return (NULL);
    }
    return (pipeline_node);
}

// Parse either a command or a group
t_ast_node *parse_command_or_group(t_list **tokens)
{
    t_list *cur;
    t_ast_node *node;

    if (!tokens || !*tokens)
        return (NULL);
    cur = *tokens;
    if (((t_org_tok *)cur->content)->oper == 2) // '('
    {
        *tokens = (*tokens)->next; // Consume "("
        node = parse_expression(tokens);
        
        if (!*tokens || ((t_org_tok *)(*tokens)->content)->oper != 3) // ')'
        {
            fprintf(stderr, "Error: Unclosed parenthesis\n");
            exit(1);
        }
        *tokens = (*tokens)->next; // Consume ")"
        return (create_group_node(node));
    }
    return (parse_command(tokens));
}

// Parse a simple command
t_ast_node *parse_command(t_list **tokens)
{
    t_list *cur;
    t_ast_node *node;

    if (!tokens || !*tokens)
        return (NULL);
    cur = *tokens;
    if (((t_org_tok *)cur->content)->oper != -2)
        return (NULL);
    node = create_command_node(((t_org_tok *)cur->content)->cmd_node);
    if (!node)
        return (NULL);
    *tokens = (*tokens)->next;
    return node;
}






















































void print_ast(t_ast_node *node, int depth)
{
	if (!node) return;
	for (int i = 0; i < depth; ++i)
		printf("  ");
	switch (node->type)
	{
		case NODE_COMMAND:
			printf("COMMAND: %s\n", node->cmd->cmd_arr[0]);
			break;
		case NODE_PIPELINE:
			printf("PIPELINE:\n");
			for (t_list *cur = node->pipeline->cmds; cur; cur = cur->next)
			{
				for (int i = 0; i < depth + 1; ++i)
					printf("  ");
				t_cmd_node *cmd = (t_cmd_node *)cur->content;
				if (cmd && cmd->cmd_arr)
					printf("CMD: %s\n", cmd->cmd_arr[0]);
			}
			break;
		case NODE_LOGICAL:
			printf("LOGICAL: %s\n", node->logical->op == OP_AND ? "&&" : "||");
			print_ast(node->logical->left, depth + 1);
			print_ast(node->logical->right, depth + 1);
			break;
		case NODE_SUBSHELL:
			printf("SUBSHELL:\n");
			print_ast(node->subshell->body, depth + 1);
			break;
		default:
			printf("Unknown node type %d\n", node->type);
	}
}