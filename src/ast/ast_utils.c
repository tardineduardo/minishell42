/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:17:52 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/21 14:04:23 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/ast.h"

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

void	free_block_node(void *ptr)
{
	t_block_node	*blk;
	
	blk = (t_block_node *)ptr;
	if (!blk)
		return;
	// here I only free cmd_arr because I manipulate it and I dont know why when edu frees it 
	// it doesnt work there, but here works... needs a better understand
	ft_free_and_null_str_array(&blk->cmd_arr);
	// ft_lstclear(&blk->input_lst, free);
	// ft_lstclear(&blk->output_lst, free);
	// ft_lstclear(&blk->redirs_lst, free);
	free(blk);
}

//Free AST memory
void	free_ast(t_ast_node *node)
{
	t_block_node	*blk;
	t_pipe_info		*pipe;
	t_logical_data	*log;
	t_subshell_data	*sub;

	if (!node)
		return;

	if (node->type == NODE_COMMAND)
	{
		blk = node->block_node;
		if (blk)
			free_block_node(blk);
	}
	else if (node->type == NODE_PIPELINE)
	{
		pipe = node->pipeline;
		if (pipe)
		{
			ft_lstclear(&pipe->cmds, free_block_node);
			free(pipe);
		}
	}
	else if (node->type == NODE_LOGICAL)
	{
		log = node->logical;
		if (log)
		{
			free_ast(log->left);
			free_ast(log->right);
			free(log);
		}
	}
	else if (node->type == NODE_SUBSHELL)
	{
		sub = node->subshell;
		if (sub)
		{
			free_ast(sub->body);
			free(sub);
		}
	}
	free(node);
	//node = NULL;   falar com edu sobre isso aqui TODO
	//node = NULL;
}

void	ft_clear_ast_mem(t_ast_mem **ast)
{
	free_ast((*ast)->root);
	free(*ast);
	return ;
}
