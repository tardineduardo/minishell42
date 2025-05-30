/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toksing.c                                          :+:      :+:    :+:   */
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

int	ft_check_syntax(t_dlist *toklst, t_tok_mem **tok)
{
	t_dlist		*trav;

	trav = toklst;
	while (trav)
	{
		if (!operators_are_supported(trav, tok))
			return (1);
		if (!redirects_are_complete(trav, tok))
			return (1);
		if (!subshell_opers_are_correct(trav, tok))
			return (1);
		if (!logic_opers_are_correct(trav, tok))
			return (1);
		if (!pipe_opers_are_correct(trav, tok))
			return (1);
		trav = trav->next;
	}
	return (0);
}

int	ft_tok_syntax_error(int st_err, char *str, t_tok_mem **tok)
{
	(*tok)->errnmb = st_err;
	if (st_err == E_NO_SUPPRT)
		ft_dprintf(STDERR_FILENO,
			"minishell: operator not supported `%s'\n", str);
	else if (st_err == E_INVAL_OPS)
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n", str);
	else if (st_err == E_NO_SUBSHE)
		ft_dprintf(STDERR_FILENO,
			"minishell: subshell `%s' not supported\n", str);
	return (false);
}
