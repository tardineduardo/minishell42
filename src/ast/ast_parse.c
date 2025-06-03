/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:01:22 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 23:23:06 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/parsing.h"

// Parse logical OR (||)
t_ast_node	*parse_logical_or(t_list **parlst, t_mem **mem)
{
	t_ast_node	*node;
	t_ast_node	*right;

	node = parse_logical_and(parlst, mem);
	if (!node)
		return (NULL);
	while (*parlst && ((t_par_node *)(*parlst)->content)->oper == 1)
	{
		*parlst = (*parlst)->next;
		right = parse_logical_and(parlst, mem);
		if (!right)
			return (NULL);
		node = create_logical_node(OP_OR, node, right);
	}
	return (node);
}

// Parse logical AND (&&)
t_ast_node	*parse_logical_and(t_list **parlst, t_mem **mem)
{
	t_ast_node	*node;
	t_ast_node	*right;

	node = parse_pipeline(parlst, mem);
	if (!node)
		return (NULL);
	while (*parlst && ((t_par_node *)(*parlst)->content)->oper == 0)
	{
		*parlst = (*parlst)->next;
		right = parse_pipeline(parlst, mem);
		if (!right)
			return (NULL);
		node = create_logical_node(OP_AND, node, right);
	}
	return (node);
}

// Parse pipeline (|)
t_ast_node	*parse_pipeline(t_list **parlst, t_mem **mem)
{
	t_ast_node	*node;
	t_list		*cmds_lst;
	t_ast_node	*pipeline_node;

	node = parse_command_or_group(parlst, mem);
	if (!node)
		return (NULL);
	if (!*parlst || ((t_par_node *)(*parlst)->content)->oper != 4)
		return (node);
	cmds_lst = ft_iterative_pipeline_parse(parlst, mem, node);
	if (!cmds_lst)
		return (NULL);
	pipeline_node = create_pipeline_node(cmds_lst);
	if (!pipeline_node)
	{
		ft_lstclear(&cmds_lst, NULL);
		return (NULL);
	}
	return (pipeline_node);
}

// Parse either a command or a group
t_ast_node	*parse_command_or_group(t_list **parlst, t_mem **mem)
{
	t_list		*cur;
	t_ast_node	*node;

	if (!parlst || !*parlst)
		return (NULL);
	cur = *parlst;
	if (((t_par_node *)cur->content)->oper == 2)
	{
		*parlst = (*parlst)->next;
		node = parse_expression(parlst, mem);
		if (!*parlst || ((t_par_node *)(*parlst)->content)->oper != 3)
		{
			ft_dprintf(2, " Error: Unclosed parenthesis\n");
			exit(1);
		}
		*parlst = (*parlst)->next;
		return (create_group_node(node));
	}
	return (parse_command(parlst, mem));
}

// Parse a simple command
t_ast_node	*parse_command(t_list **parlst, t_mem **mem)
{
	t_list		*cur;
	t_ast_node	*node;

	(void)mem;
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
