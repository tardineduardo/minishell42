/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_steps.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/expand.h"
#include "../../include/parsing.h"
#include "../../include/environs.h"
#include "../../include/readline.h"

// used by heredoc
// used by token->process_unquoted_sequence
// used by token->process_inside_double_quoted
bool	handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
{
	t_exit	exit;

	if (CURRENT_CHAR != '$' || NEXT_CHAR == '\0' || ft_isspace(NEXT_CHAR))
		return (false);
	if (NEXT_CHAR == '{')
	{
		(*exp)->braces = true;
		skip_char_no_copy(exp);
	}
	if (NEXT_CHAR && (ft_isalnum(NEXT_CHAR) || NEXT_CHAR == '?'))
	{
		exit = try_to_expand_variable(exp, mem);
		if (!exit)
		{
			(*exp)->exit = exit;
			return (false);					// INDICA ALGUM SYS ERROR 
		}
		if (exit == VAR_FOUND)
		{
			copy_value_and_increment(exp);
			return (true);
		}
		return (true);						// VARIABLE NOT FOUND, NAO É ERRO
	}
	return (false);
}

// used by token->process_unquoted_sequence
// used by token->process_inside_double_quoted -> NOT DELIM
// used by delim->process_unquoted_sequence - -> NOT DELIM
// used by heredoc->process_unquoted_sequence
bool	handle_backslash(t_exp_mem **exp, t_mode mode, t_quote quote)
{
	if (CURRENT_CHAR != '\\' || NEXT_CHAR == '\0')
		return (false);
	if (mode == TOKEN && quote == Q_OFF)
	{
		if (NEXT_CHAR == '\\')
			return (copy_char_copy_next_and_increment(exp));
		else
			return (skip_slash_copy_next_and_increment(exp));
	}
	if (mode == TOKEN && quote == Q_DOUBLE)
	{
		if (ft_strchr("\\\"\'$", NEXT_CHAR))
			return (skip_slash_copy_next_and_increment(exp));
	}
	if (mode == HEREDOC && (*exp)->hd_mode == EXPAND)
	{
		if (ft_strchr("\\$", NEXT_CHAR))
			return (skip_slash_copy_next_and_increment(exp));
	}
	return (false);
}

t_exit	try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	char	**value;
	t_exit	exit;

	value = &(*exp)->value;
	exit = get_variable_value(&CURRENT_CHAR, value, mem);
	if (exit == VAR_FOUND)
		return (insert_var_in_string(*value, (*exp)->a, exp));
	else if (exit == VAR_NOT_FOUND)
		return (remove_var_from_string(&(*exp)->raw, (*exp)->a));
	else if (exit == BAD_SUBSTITUITION)
		return (BAD_SUBSTITUITION);
	else
		return (ERROR);
}
