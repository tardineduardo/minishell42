/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:01:22 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/16 15:20:33 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"

// Parse an expression
t_ast_node	*parse_expression(t_list **parlst)
{
	return (parse_logical_or(parlst));
}

// Parse logical OR (||)
t_ast_node	*parse_logical_or(t_list **parlst)
{
	t_ast_node	*node;
	t_ast_node	*right;

	node = parse_logical_and(parlst);
	if (!node)
		return (NULL);
	while (*parlst && ((t_par_node *)(*parlst)->content)->oper == 1)
	{
		*parlst = (*parlst)->next;
		right = parse_logical_and(parlst);
		if (!right)
			return (NULL);
		node = create_logical_node(OP_OR, node, right);
	}
	return (node);
}

// Parse logical AND (&&)
t_ast_node	*parse_logical_and(t_list **parlst)
{
	t_ast_node	*node;
	t_ast_node	*right;

	node = parse_pipeline(parlst);
	if (!node)
		return (NULL);
	while (*parlst && ((t_par_node *)(*parlst)->content)->oper == 0)
	{
		*parlst = (*parlst)->next;
		right = parse_pipeline(parlst);
		if (!right)
			return (NULL);
		node = create_logical_node(OP_AND, node, right);
	}
	return (node);
}

// Parse pipeline (|)
t_ast_node	*parse_pipeline(t_list **parlst)
{
	t_ast_node	*node;
	t_list		*cmds_lst;
	t_ast_node	*right;
	t_list		*new_node;
	t_ast_node	*pipeline_node;
	t_list		*next;

	node = parse_command_or_group(parlst);
	if (!node)
		return (NULL);
	// If we don't have a pipe, return the single command
	if (!*parlst || ((t_par_node *)(*parlst)->content)->oper != 4)
		return (node);
	// We have a pipeline - create a list of commands
	cmds_lst = ft_lstnew(node->block_node);
	if (!cmds_lst)
		return (NULL);
	while (*parlst && ((t_par_node *)(*parlst)->content)->oper == 4)
	{
		*parlst = (*parlst)->next; // Consume '|'
		right = parse_command_or_group(parlst);
		if (!right || right->type != NODE_COMMAND)
		{
			ft_dprintf(2, " Invalid pipeline structure\n");
			exit(1);
		}
		new_node = ft_lstnew(right->block_node);
		if (!new_node)
		{
			// Free the list we created
			while (cmds_lst)
			{
				next = cmds_lst->next;
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
			next = cmds_lst->next;
			free(cmds_lst);
			cmds_lst = next;
		}
		return (NULL);
	}
	return (pipeline_node);
}

// Parse either a command or a group
t_ast_node	*parse_command_or_group(t_list **parlst)
{
	t_list		*cur;
	t_ast_node	*node;

	if (!parlst || !*parlst)
		return (NULL);
	cur = *parlst;
	if (((t_par_node *)cur->content)->oper == 2) // '('
	{
		*parlst = (*parlst)->next; // Consume "("
		node = parse_expression(parlst);
		if (!*parlst || ((t_par_node *)(*parlst)->content)->oper != 3) // ')'
		{
			ft_dprintf(2, " Error: Unclosed parenthesis\n");
			exit(1);
		}
		*parlst = (*parlst)->next; // Consume ")"
		return (create_group_node(node));
	}
	return (parse_command(parlst));
}

// Parse a simple command
t_ast_node	*parse_command(t_list **parlst)
{
	t_list		*cur;
	t_ast_node	*node;

	if (!parlst || !*parlst)
		return (NULL);
	cur = *parlst;
	if (((t_par_node *)cur->content)->block_index == -1)
		return (NULL);
	node = create_command_node(((t_par_node *)cur->content)->block_node);
	if (!node)
		return (NULL);
	*parlst = (*parlst)->next;
	return (node);
}

// void print_ast(t_ast_node *node, int depth)
// {
// 	if (!node) return;
// 	for (int i = 0; i < depth; ++i)
// 		printf("  ");
// 	switch (node->type)
// 	{
// 		case NODE_COMMAND:
// 			printf("COMMAND: %s\n", node->block_node->cmd_arr[0]);
// 			break;
// 		case NODE_PIPELINE:
// 			printf("PIPELINE:\n");
// 			for (t_list *cur = node->pipeline->cmds; cur; cur = cur->next)
// 			{
// 				for (int i = 0; i < depth + 1; ++i)
// 					printf("  ");
// 				t_block_node *block_node = (t_block_node *)cur->content;
// 				if (block_node && block_node->cmd_arr)
// 					printf("CMD: %s\n", block_node->cmd_arr[0]);
// 			}
// 			break;
// 		case NODE_LOGICAL:
// 			printf("LOGICAL: %s\n", node->logical->op == OP_AND ? "&&" : "||");
// 			print_ast(node->logical->left, depth + 1);
// 			print_ast(node->logical->right, depth + 1);
// 			break;
// 		case NODE_SUBSHELL:
// 			printf("SUBSHELL:\n");
// 			print_ast(node->subshell->body, depth + 1);
// 			break;
// 		default:
// 			printf("Unknown node type %d\n", node->type);
// 	}
// }