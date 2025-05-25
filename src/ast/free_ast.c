/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:49:10 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/25 16:03:37 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/parsing.h"

void	free_block_node(void *ptr)
{
	t_block_node	*blk;

	blk = (t_block_node *)ptr;
	if (!blk)
		return ;
	ft_free_and_null_str_array(&blk->cmd_arr);
}

//Free AST memory
void	ft_free_ast(t_ast_node *node)
{
	t_block_node	*blk;
	t_pipe_info		*pipe;
	t_logical_data	*log;
	t_subshell_data	*sub;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		blk = node->block_node;
		if (blk)
		{
			free_block_node(blk);
			free(blk);
		}
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
			ft_free_ast(log->left);
			ft_free_ast(log->right);
			free(log);
		}
	}
	else if (node->type == NODE_SUBSHELL)
	{
		sub = node->subshell;
		if (sub)
		{
			ft_free_ast(sub->body);
			free(sub);
		}
	}
	free(node);
}

void	ft_clear_ast_mem(t_ast_mem **ast)
{
	free(*ast);
	return ;
}
