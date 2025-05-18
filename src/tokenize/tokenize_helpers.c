/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/parsing.h"

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

bool	ft_is_oper_token(char *str, t_tok_mem **tok, int *operator_len)
{
	int		i;
	char	*curr_operator;

	i = 0;
	while ((*tok)->operators[i])
	{
		curr_operator = (*tok)->operators[i];
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
