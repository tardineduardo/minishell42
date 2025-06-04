/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_limits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 00:59:12 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

static int	ft_find_word_limit(t_tok_mem **tok, char *str)
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
		else if ((ft_isspace(str[i]) || ft_is_oper_token((&str[i]), tok, NULL))
			&& (*tok)->quote == Q_OFF)
			return (i);
		i++;
	}
	return (i);
}

static int	ft_handle_quotes(char *str, t_tok_mem **tok, int *i)
{
	if (ft_is_single_quote(&str[*i]) || ft_is_double_quote(&str[*i]))
	{
		*i += ft_find_word_limit(tok, &str[*i]);
		return (1);
	}
	return (0);
}

static int	ft_handle_operator(char *str, t_tok_mem **tok, int i)
{
	int	operator_len;

	if (ft_is_oper_token(&str[i], tok, &operator_len))
	{
		if (i == 0)
			return (operator_len);
		return (i);
	}
	return (-1);
}

int	ft_find_token_limit(char *str, t_tok_mem **tok)
{
	int	i;
	int	op_len;

	i = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			break ;
		if (ft_handle_quotes(str, tok, &i))
			continue ;
		if (str[i] == '\\')
		{
			i += 2;
			continue ;
		}
		op_len = ft_handle_operator(str, tok, i);
		if (op_len != -1)
			return (op_len);
		i++;
	}
	return (i);
}
