/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_syntax_cases.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:35:26 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/31 23:36:57 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

bool	ft_operators_are_supported(t_dlist *trav, t_tok_mem **tok)
{
	t_tok_node	*tknd;

	tknd = (t_tok_node *)trav->content;
	if (tknd->oper != WORD)
	{
		if (tknd->oper != PIPE_O && tknd->oper != OUT_R
			&& tknd->oper != IN_R && tknd->oper != APPD_R
			&& tknd->oper != HDC_R && tknd->oper != AND_O
			&& tknd->oper != OR_O && tknd->oper != GSTART_O
			&& tknd->oper != GEND_O)
			return (ft_tok_syntax_error(E_NO_SUPPRT, ft_getop(tknd), tok));
		trav = trav->next;
	}
	return (true);
}

bool	ft_redirects_are_complete(t_dlist *trav, t_tok_mem **tok)
{
	t_tok_node	*tknd;
	t_tok_node	*next;

	tknd = (t_tok_node *)trav->content;
	if (ft_is_redir(tknd))
	{
		if (!trav->next)
			return (ft_tok_syntax_error(E_INVAL_OPS, "newline", tok));
		next = (t_tok_node *)trav->next->content;
		if (!next || next->oper != WORD)
			return (ft_tok_syntax_error(E_INVAL_OPS, ft_getop(next), tok));
	}
	return (true);
}

bool	ft_subshell_opers_are_correct(t_dlist *trav, t_tok_mem **tok)
{
	t_tok_node	*tknd;
	t_tok_node	*next;

	tknd = (t_tok_node *)trav->content;
	if (tknd->oper == GSTART_O)
	{
		if (!trav->next)
			return (ft_tok_syntax_error(E_INVAL_OPS, "newline", tok));
		next = (t_tok_node *)trav->next->content;
		if (!ft_is_word(next))
			return (ft_tok_syntax_error(E_INVAL_OPS, ft_getop(next), tok));
		if (next->oper == GSTART_O)
			return (ft_tok_syntax_error(E_NO_SUBSHE, "((", tok));
	}
	return (true);
}

bool	ft_logic_opers_are_correct(t_dlist *trav, t_tok_mem **tok)
{
	t_tok_node	*tknd;
	t_tok_node	*next;

	tknd = (t_tok_node *)trav->content;
	if (tknd->oper == AND_O || tknd->oper == OR_O)
	{
		if (!trav->prev)
			return (ft_tok_syntax_error(E_INVAL_OPS, ft_getop(tknd), tok));
		if (!trav->next)
			return (ft_tok_syntax_error(E_INVAL_OPS, "newline", tok));
		next = (t_tok_node *)trav->next->content;
		if (!ft_is_word(next) && next->oper != GSTART_O)
			return (ft_tok_syntax_error(E_INVAL_OPS, ft_getop(next), tok));
	}
	return (true);
}

bool	ft_pipe_opers_are_correct(t_dlist *trav, t_tok_mem **tok)
{
	t_tok_node	*tknd;
	t_tok_node	*next;

	tknd = (t_tok_node *)trav->content;
	if (tknd->oper == PIPE_O)
	{
		if (!trav->prev)
			return (ft_tok_syntax_error(E_INVAL_OPS, ft_getop(tknd), tok));
		if (!trav->next)
			return (ft_tok_syntax_error(E_INVAL_OPS, "newline", tok));
		next = (t_tok_node *)trav->next->content;
		if (next->oper == GEND_O || next->oper == AND_O
			|| next->oper == OR_O)
			return (ft_tok_syntax_error(E_INVAL_OPS, ft_getop(next), tok));
		if (next->oper == PIPE_O)
			return (ft_tok_syntax_error(E_INVAL_OPS, ft_getop(next), tok));
	}
	return (true);
}
