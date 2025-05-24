/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_limits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/24 10:06:07 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/parsing.h"

int	ft_find_token_limit(char *str, t_tok_mem **tok)
{
	int		i;
	int		operator_len;

	i = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			break ;
		if (ft_is_single_quote(&str[i]) || ft_is_double_quote(&str[i]))
		{
			i += ft_find_word_limit(tok, &str[i]);
			continue ;
		}
		if (ft_isspace(str[i]))
			break ;
		if (str[i] == '\\')
			ft_tokeniztion_escape(&i);
		if (ft_isspace(str[i]))
			break ;
		if (ft_is_oper_token(&str[i], &operator_len))
		{
			if (i == 0)
				return (operator_len);
			return (i);
		}
		i++;
	}
	return (i);
}

int	ft_find_word_limit(t_tok_mem **tok, char *str)
{
	int	i;

	if (str[0] == '\'')
		(*tok)->quote = Q_SINGLE;
	else if (str[0] == '\"')
		(*tok)->quote = Q_DOUBLE;
	i = 1;
	while (str[i])
	{
		if ((ft_is_single_quote(&str[i]) && (*tok)->quote == Q_SINGLE)
			|| (ft_is_double_quote(&str[i]) && (*tok)->quote == Q_DOUBLE))
			(*tok)->quote = Q_OFF;
		else if ((ft_is_single_quote(&str[i]) && (*tok)->quote == Q_OFF))
			(*tok)->quote = Q_SINGLE;
		else if ((ft_is_double_quote(&str[i]) && (*tok)->quote == Q_OFF))
			(*tok)->quote = Q_DOUBLE;
		else if ((ft_isspace(str[i]) || ft_is_oper_token((&str[i]), NULL))
			&& (*tok)->quote == Q_OFF)
			return (i);
		i++;
	}
	return (i);
}
