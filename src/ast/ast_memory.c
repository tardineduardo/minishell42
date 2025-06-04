/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_memory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:49:10 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/03 17:43:59 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/parsing.h"

void	ft_del_redirs_nodes(void *content)
{
	t_redirs_node	*redir_node;

	if (!content)
		return ;
	redir_node = (t_redirs_node *)content;
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

void	ft_del_cmd_nodes(void *content)
{
	t_cmd_node	*cmdnode;

	if (!content)
		return ;
	cmdnode = (t_cmd_node *)content;
	if (cmdnode->cmdvalue)
		free(cmdnode->cmdvalue);
	free(cmdnode);
}

void	ft_free_block_node(void *ptr)
{
	t_block_node	*blk;

	blk = (t_block_node *)ptr;
	if (!blk)
		return ;
	if (blk->redirs_lst)
		ft_lstclear(&blk->redirs_lst, ft_del_redirs_nodes);
	if (blk->output_lst)
		ft_lstclear(&blk->output_lst, NULL);
	if (blk->input_lst)
		ft_lstclear(&blk->input_lst, NULL);
	if (blk->cmd_arr)
		ft_free_and_null_str_array(&blk->cmd_arr);
	if (blk->cmd_lst)
		ft_lstclear(&blk->cmd_lst, ft_del_cmd_nodes);
	free(blk);
}

void	ft_free_ast(t_ast_node **node)
{
	if (!(*node))
		return ;
	if ((*node)->type == NODE_COMMAND)
		ft_free_command(node);
	else if ((*node)->type == NODE_PIPELINE)
		ft_free_pipe(node);
	else if ((*node)->type == NODE_LOGICAL)
		ft_free_logical(node);
	else if ((*node)->type == NODE_SUBSHELL)
		ft_free_subshell(node);
	ft_free_and_null((void **)node);
}

void	ft_clear_ast_mem(t_ast_mem **ast)
{
	if ((*ast)->root)
		ft_free_ast(&(*ast)->root);
	free(*ast);
	return ;
}
