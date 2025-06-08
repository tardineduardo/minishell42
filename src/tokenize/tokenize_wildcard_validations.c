/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard_validations.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/08 14:52:01 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"
#include "../../include/readline.h"

static bool	ft_is_asterisk(char *token)
{
	while (*token)
	{
		if (*token != '*' && *token != ' ' && *token != '\t')
			return (false);
		token++;
	}
	return (true);
}

static bool	ft_has_invalid_ops(char *st, t_tok_mem **tkmem)
{
	int	a;

	a = 0;
	while (st[a] != '\0')
	{
		if (st[a] == '\"' || st[a] == '\'' || st[a] == '$')
		{
			ft_dprintf(STDERR_FILENO, "minishell: error: quotes and expansion");
			ft_dprintf(STDERR_FILENO, " not allowed in wildcard tokens.\n");
			(*tkmem)->errnmb = 1;
			return (true);
		}
		a++;
	}
	return (false);
}

static bool	ft_mixes_ast_and_quotes(char *st)
{
	int		a;
	t_quote	quote;

	a = 0;
	quote = Q_OFF;
	if (!ft_strchr(st, '\'') && !ft_strchr(st, '\"'))
		return (false);
	while (st[a] != '\0')
	{
		ft_update_quote_flag_rdl(st[a], &quote, a, st);
		if (st[a] == '*' && (quote == Q_DOUBLE || quote == Q_SINGLE))
			return (true);
		a++;
	}
	return (false);
}

static t_wcexit	ft_validations_wc(t_tok_node *currtok, char *st,
					t_tok_mem **tkmem)
{
	if (!ft_strchr(currtok->value, '*'))
		return (W_NO_WILD);
	if (ft_is_asterisk(st))
		return (W_SUCCESS);
	if (ft_mixes_ast_and_quotes(st))
		return (W_ERROR);
	if (ft_has_invalid_ops(st, tkmem))
		return (W_ERROR);
	return (W_SUCCESS);
}

t_wcexit	ft_token_has_valid_wildcard(t_dlist *trav, t_tok_mem **tkmem)
{
	t_tok_node	*currtok;
	t_wcexit	res;
	int			ct;
	char		*st;

	currtok = (t_tok_node *)trav->content;
	if (!currtok)
	{
		(*tkmem)->errnmb = 1;
		return (W_ERROR);
	}
	st = currtok->value;
	res = ft_validations_wc(currtok, st, tkmem);
	if (res != W_SUCCESS)
		return (res);
	ct = ft_split_count(currtok->value, '*');
	if (ct > 2 || (ct == 2 && (st[0] == '*' || st[ft_strlen(st) - 1] == '*')))
	{
		ft_dprintf(STDERR_FILENO, "minishell: error: invalid wildcard format.");
		ft_dprintf(STDERR_FILENO, " use *pat, pat*, *pat* or pat*pat.\n");
		(*tkmem)->errnmb = 1;
		return (W_ERROR);
	}
	return (W_SUCCESS);
}
