/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_valids.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/06 18:59:29 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"

bool	ft_is_redir(t_tok_node *toknode)
{
	if (toknode->oper == IN_R || toknode->oper == OUT_R
		|| toknode->oper == HDC_R || toknode->oper == APPD_R)
		return (true);
	if (toknode->oper == WILD_R || toknode->oper == ERROR_R
		|| toknode->oper == HSTR_R || toknode->oper == OERR_R)
		return (true);
	return (false);
}

bool	ft_is_word(t_tok_node *toknode)
{
	if (toknode->oper == WORD)
		return (true);
	return (false);
}

bool	ft_is_command(t_tok_node *toknode)
{
	if (toknode->oper == WORD)
		return (true);
	if (toknode->oper == IN_R || toknode->oper == OUT_R
		|| toknode->oper == HDC_R || toknode->oper == APPD_R)
		return (true);
	if (toknode->oper == WILD_R || toknode->oper == ERROR_R
		|| toknode->oper == HSTR_R || toknode->oper == OERR_R)
		return (true);
	return (false);
}

bool	ft_is_pipe_logical_subshell(t_tok_node *toknode)
{
	if (toknode->oper == PIPE_O || toknode->oper == GSTART_O
		|| toknode->oper == GEND_O || toknode->oper == AND_O
		|| toknode->oper == OR_O)
		return (true);
	return (false);
}
