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

bool	ft_check_syntax(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;

	trav = toklst;
	while (trav)
	{
		if (!operators_are_supported(trav, par))
			return (false);
		if (!redirects_are_complete(trav, par))
			return (false);
		if (!subshell_opers_are_correct(trav, par))
			return (false);
		if (!logic_opers_are_correct(trav, par))
			return (false);
		if (!pipe_opers_are_correct(trav, par))
			return (false);
		trav = trav->next;
	}
	return (true);
}
