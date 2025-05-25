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

void	*ft_par_syntax_error(int st_err, char *str, t_par_mem **par)
{
	(*par)->errnmb = st_err;
	if (st_err == E_NO_SUPPRT)
		ft_dprintf(STDERR_FILENO,
			"minishell: operator not supported `%s'\n", str);
	else if (st_err == E_INVAL_OPS)
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n", str);
	else if (st_err == E_NO_SUBSHE)
		ft_dprintf(STDERR_FILENO,
			"minishell: subshell `%s' not supported\n", str);
	return (NULL);
}

void	*ft_par_syscall_error(t_par_mem **par, char *ftname)
{
	(*par)->errnmb = errno;
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", ftname, strerror(errno));
	return (NULL);
}
