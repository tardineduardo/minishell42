/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_memory_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:43:28 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/06 18:15:23 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/parsing.h"

void	ft_free_command(t_ast_node **node)
{
	t_block_node	*blk;

	if ((*node)->type == NODE_COMMAND)
	{
		blk = (*node)->block_node;
		if (blk)
			ft_free_block_node(blk);
	}
}

void	ft_free_ast_content(void *content)
{
	t_ast_node	*node;

	node = (t_ast_node *)content;
	if (node)
	{
		if (node->type == NODE_COMMAND)
			ft_free_block_node(node->block_node);
		else if (node->type == NODE_SUBSHELL)
		{
			if (node->subshell)
			{
				ft_free_ast(&node->subshell->body);
				free(node->subshell);
			}
		}
		free(node);
	}
}

void	ft_free_pipe(t_ast_node **node)
{
	t_pipe_info	*pipe;

	if ((*node)->type == NODE_PIPELINE)
	{
		pipe = (*node)->pipeline;
		if (pipe)
		{
			ft_lstclear(&pipe->cmds, ft_free_ast_content);
			free(pipe);
		}
	}
}

void	ft_free_logical(t_ast_node **node)
{
	t_logical_data	*log;

	if ((*node)->type == NODE_LOGICAL)
	{
		log = (*node)->logical;
		if (log)
		{
			ft_free_ast(&log->left);
			ft_free_ast(&log->right);
			free(log);
		}
	}
}

void	ft_free_subshell(t_ast_node **node)
{
	t_subshell_data	*sub;

	if ((*node)->type == NODE_SUBSHELL)
	{
		sub = (*node)->subshell;
		if (sub)
		{
			ft_free_ast(&sub->body);
			free(sub);
		}
	}
}
