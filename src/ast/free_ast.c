/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:49:10 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/30 23:04:20 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/parsing.h"

void	ft_del_redirs_nodes(void **content)
{
	t_redirs_node	*redir_node;

	if (!(*content))
		return ;
	redir_node = (t_redirs_node *)(*content);
	if (redir_node->type == HDC_R)
	{
		if (redir_node->name != NULL)
			unlink(redir_node->name);
	}
	if (redir_node->name)
		ft_free_and_null((void *)&redir_node->name);
	if (redir_node)
		ft_free_and_null((void *)&redir_node);
}

void	free_block_node(t_block_node **node)
{
	if (!*node)
		return ;
	if ((*node)->redirs_lst)
		ft_lstclear2(&(*node)->redirs_lst, ft_del_redirs_nodes);
	if ((*node)->output_lst)
		ft_lstclear2(&(*node)->output_lst, ft_del_redirs_nodes);
	if ((*node)->input_lst)
		ft_lstclear2(&(*node)->input_lst, ft_del_redirs_nodes);
	if ((*node)->cmd_arr)
		ft_free_and_null_str_array(&(*node)->cmd_arr);
	free(*node);
}

void	free_block_node_wrapper(void **ptr)
{
	free_block_node((t_block_node **)ptr);
}

void	ft_free_ast(t_ast_node **node)
{
	t_block_node	*blk;
	t_pipe_info		*pipe;
	t_logical_data	*log;
	t_subshell_data	*sub;

	if (!(*node))
		return ;
	if ((*node)->type == NODE_COMMAND)
	{
		blk = (*node)->block_node;
		if (blk)
			free_block_node(&blk);
	}
	else if ((*node)->type == NODE_PIPELINE)
	{
		pipe = (*node)->pipeline;
		if (pipe)
		{
			ft_lstclear2(&pipe->cmds, free_block_node_wrapper);
			free(pipe);
		}
	}
	else if ((*node)->type == NODE_LOGICAL)
	{
		log = (*node)->logical;
		if (log)
		{
			ft_free_ast(&log->left);
			ft_free_ast(&log->right);
			free(log);
		}
	}
	else if ((*node)->type == NODE_SUBSHELL)
	{
		sub = (*node)->subshell;
		if (sub)
		{
			ft_free_ast(&sub->body);
			free(sub);
		}
	}
	ft_free_and_null((void**)node);
}

void	ft_clear_ast_mem(t_ast_mem **ast)
{
	if ((*ast)->root)
		ft_free_ast(&(*ast)->root);
	free(*ast);
	return ;
}
