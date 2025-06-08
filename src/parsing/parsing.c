/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 20:36:16 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

static void	*initloop(int a, t_par_node **pnd, t_dlist **tklst, t_par_mem **par)
{
	t_dlist		*first;
	t_tok_node	*toknode;

	while (1)
	{
		first = *tklst;
		if (!first)
			return (*pnd);
		toknode = (t_tok_node *)first->content;
		if (toknode->block_index > a || toknode->block_index == -1)
			return (*pnd);
		if (ft_is_redir(toknode))
		{
			if (!fill_bnode_redir(tklst, pnd, par))
				return (NULL);
		}
		else if (ft_is_word(toknode))
		{
			if (!fill_bnode_cmdsa(tklst, pnd, par))
				return (NULL);
		}
	}
	return (*pnd);
}

static t_par_node	*init_pnd(int a, t_par_node **pnd, t_dlist **tklst,
	t_par_mem **par)
{
	t_dlist		*first;
	t_tok_node	*toknode;

	first = *tklst;
	toknode = (t_tok_node *)first->content;
	(*pnd)->block_index = toknode->block_index;
	(*pnd)->block_node = NULL;
	if (ft_is_pipe_logical_subshell(toknode))
	{
		(*pnd)->oper = toknode->oper;
		ft_dlst_quick_destroy_node(tklst, *tklst, ft_del_token_node);
		return (*pnd);
	}
	if (!initloop(a, pnd, tklst, par))
		return (NULL);
	(*pnd)->oper = CMD;
	return (*pnd);
}

static int	count_num_parsnodes(t_dlist **toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;
	t_tok_node	*prevtok;
	int			total_parsnodes;

	trav = *toklst;
	total_parsnodes = 0;
	while (trav)
	{
		toknode = (t_tok_node *)trav->content;
		if (trav == *toklst)
			total_parsnodes++;
		else if (ft_is_command(toknode) && !ft_is_command(prevtok))
			total_parsnodes++;
		else if (ft_is_pipe_logical_subshell(toknode))
			total_parsnodes++;
		prevtok = toknode;
		trav = trav->next;
	}
	return (total_parsnodes);
}

static t_list	*ft_create_parlst(t_dlist **toklst, t_list **parlst,
	t_par_mem **par)
{
	t_list		*new;
	t_par_node	*parnode;
	int			num_parsnodes;
	int			a;

	num_parsnodes = count_num_parsnodes(toklst);
	a = 0;
	while (a < num_parsnodes)
	{
		parnode = malloc(sizeof(t_par_node));
		if (!parnode)
			return (ft_par_syscall_error(par, "ft_create_parlst"));
		if (!init_pnd(a, &parnode, toklst, par))
			return (NULL);
		new = ft_lstnew(parnode);
		if (!new)
			return (ft_par_syscall_error(par, "ft_create_parlst"));
		ft_lstadd_back(parlst, new);
		a++;
	}
	return (*parlst);
}

int	ft_parsing(t_mem **mem)
{
	t_par_mem	*par;
	t_tok_mem	*tok;
	t_list		*head_parlst;

	tok = (*mem)->tokenize;
	par = (*mem)->parsing;
	(*mem)->parsing->errnmb = 0;
	ft_create_parlst(&tok->toklst, &par->parlst, &par);
	if (par->errnmb == 0)
	{
		head_parlst = (*mem)->parsing->parlst;
		parse_expression(&head_parlst, mem);
	}
	return (par->errnmb);
}
