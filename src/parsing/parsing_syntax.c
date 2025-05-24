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

bool	ft_check_syntax(t_dlist *parlst, t_par_mem **par)
{
	if (!operators_are_supported(parlst, par))
		return (false);
	if (!redirects_are_complete(parlst, par))
		return (false);
	if (!subshell_opers_are_correct(parlst, par))
		return (false);
	if (!logic_opers_are_correct(parlst, par))
		return (false);
	// if (!pipe_opers_are_correct(parlst, par))
	// 	return (false);
	return (true);
}
