/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:17:52 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/06 17:55:09 by luide-ca         ###   ########.fr       */
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

t_ast_node	*create_logical_node(t_logical_op op,
	t_ast_node *left, t_ast_node *right)
{
	t_ast_node		*node;
	t_logical_data	*logical;

	node = malloc(sizeof(t_ast_node));
	logical = malloc(sizeof(t_logical_data));
	if (!node || !logical)
	{
		if (node)
			free(node);
		if (logical)
			free(logical);
		return (NULL);
	}
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
	if (!node)
		return (NULL);
	node->subshell = malloc(sizeof(t_subshell_data));
	if (!node->subshell)
	{
		free(node);
		return (NULL);
	}
	node->type = NODE_SUBSHELL;
	node->subshell->body = body;
	return (node);
}
