/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/24 10:05:30 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/parsing.h"

const char *g_operators[] = {"<<<",">>", "<<", "&&", "||", "2>", "&>", "<",
	">", "|", "&", "*", "(", ")"};

void	ft_tokeniztion_escape(int *i)
{
	(*i)++;
	(*i)++;
	return ;
}

char	*getop(t_tok_node *tok)
{
	if (tok->oper == AND_O)
		return ("&&");
	if (tok->oper == OR_O)
		return ("||");
	if (tok->oper == GSTART_O)
		return ("(");
	if (tok->oper == GEND_O)
		return (")");
	if (tok->oper == PIPE_O)
		return ("|");
	if (tok->oper == IN_R)
		return ("<");
	if (tok->oper == OUT_R)
		return (">");
	if (tok->oper == APPD_R)
		return (">>");
	if (tok->oper == HDC_R)
		return ("<<");
	if (tok->oper == WILD_R)
		return ("*");
	else
		return (NULL);
}

bool	ft_is_oper_token(char *str, int *operator_len)
{
	int			i;
	const char	*curr_operator;

	i = 0;
	while (g_operators[i])
	{
		curr_operator = g_operators[i];
		if (ft_strncmp(str, curr_operator, ft_strlen(curr_operator)) == 0)
		{
			if (operator_len)
				*operator_len = ft_strlen(curr_operator);
			return (true);
		}
		i++;
	}
	return (false);
}
