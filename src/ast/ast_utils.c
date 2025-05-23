/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:17:52 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/16 18:05:11 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"

// Create a new command node
t_ast_node	*create_command_node(t_block_node *block_node)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->block_node = block_node;
	return (node);
}

t_ast_node	*create_pipeline_node(t_list *cmds_lst)
{
	t_ast_node	*node;
	t_pipe_info	*pipe_data;
	t_list		*current;

	node = malloc(sizeof(t_ast_node));
	pipe_data = malloc(sizeof(t_pipe_info));
	if (!node || !pipe_data)
	{
		if (node)
			free(node);
		if (pipe_data)
			free(pipe_data);
		return (NULL);
	}
	node->type = NODE_PIPELINE;
	pipe_data->cmds = cmds_lst;
	pipe_data->cmd_count = 0;
	current = cmds_lst;
	while (current)
	{
		pipe_data->cmd_count++;
		current = current->next;
	}
	node->pipeline = pipe_data;
	return (node);
}

t_ast_node	*create_logical_node(logical_op op, t_ast_node *left, t_ast_node *right)
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

t_ast_node	*create_group_node(t_ast_node *body)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	node->subshell = malloc(sizeof(t_subshell_data));
	node->type = NODE_SUBSHELL;
	node->subshell->body = body;
	return (node);
}


// Free AST memory
// void free_ast(t_ast_node *node)
// {
// 	int	i;

// 	i = 0;
//     if (!node)
// 		return ;
//     if (node->type == NODE_PIPELINE)
// 	{
// 		while (i < node->pipeline->cmd_count)
// 		{
// 			free_block_node(((t_block_node *)node->pipeline->cmds->content));
// 			i++;
// 		}
// 		free(node->pipeline->cmds);
// 	}
// 	else if (node->type == NODE_COMMAND)
//         free_block_node(node->block_node);
// 	else if (node->type == NODE_LOGICAL)
// 	{
// 		free_ast(node->logical->left);
// 		free_ast(node->logical->right);
// 	}
// 	else if (node->type == NODE_SUBSHELL)
// 		free_ast(node->subshell->body);
//     free(node);
// }