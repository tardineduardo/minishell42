/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/19 12:43:10 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/checks.h"

void	*operators_are_supported(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
		if (tknd->oper != WORD)
		{
			if (tknd->oper != PIPE_O
				&& tknd->oper != OUT_R
				&& tknd->oper != IN_R
				&& tknd->oper != APPD_R
				&& tknd->oper != HDC_R
				&& tknd->oper != AND_O
				&& tknd->oper != OR_O
				&& tknd->oper != GSTART_O
				&& tknd->oper != GEND_O)
				return (ft_par_syntax_error(E_NO_SUPPRT, getop(tknd), par));
		}
		trav = trav->next;
	}
	return (toklst);
}

void	*redirects_are_complete(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
		if (is_redir(tknd))
		{
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (!next || next->oper != WORD)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));
		}
		trav = trav->next;
	}
	return (toklst);
}

void	*subshell_opers_are_correct(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
		if (tknd->oper == GSTART_O)
		{
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (!is_word(next))
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));
			if (next->oper == GSTART_O)
				return (ft_par_syntax_error(E_NO_SUBSHE, "((", par));
		}
		trav = trav->next;
	}
	return (toklst);
}

void	*logic_opers_are_correct(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
		if (tknd->oper == AND_O || tknd->oper == OR_O)
		{
			if (!trav->prev)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(tknd), par));
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (!is_word(next) && next->oper != GSTART_O)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));
		}
		trav = trav->next;
	}
	return (toklst);
}

void	*pipe_opers_are_correct(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
		if (tknd->oper == PIPE_O)
		{
			if (!trav->prev)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(tknd), par));
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (next->oper == GEND_O || next->oper == AND_O
				|| next->oper == OR_O)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));
		}
		trav = trav->next;
	}
	return (toklst);
}
