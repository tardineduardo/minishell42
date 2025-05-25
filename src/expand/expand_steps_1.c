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

// used by delim, token
void	update_quote_flag(char *s, t_quote *quote, int index)
{
	char	c;

	c = s[index];
	if (!ft_isquote(c))
		return ;
	if (!is_char_escaped(s, index))
	{
		if (c == '\'' && *quote == Q_OFF)
			*quote = Q_SINGLE;
		else if (c == '\"' && *quote == Q_OFF)
			*quote = Q_DOUBLE;
		else if (c == '\'' && *quote == Q_SINGLE)
			*quote = Q_OFF;
		else if (c == '\"' && *quote == Q_DOUBLE)
			*quote = Q_OFF;
	}
	return ;
}

// used by delim, token
bool	skip_if_quote_changed(t_exp_mem **exp, t_quote *quote, t_quote *prev)
{
	if (*quote != *prev)
	{
		skip_char_no_copy(exp);
		return (true);
	}
	return (false);
}

// used by delim, token
bool	process_inside_single_quotes(t_exp_mem **exp, t_quote quote)
{
	if (quote == Q_SINGLE)
	{
		while (!is_closing_quote(CURRENT_CHAR, &quote))
			copy_char_and_increment(exp);
		return (true);
	}
	return (false);
}

// used by delim, token
bool	process_inside_double_quote(t_exp_mem **exp, t_mem **mem, t_quote quote)
{
	t_mode	mode;

	mode = (*exp)->mode;
	if (quote == Q_DOUBLE)
	{
		if (mode == TOKEN)
		{
			if (handle_dollar_sign(exp, mem))
			{
				if ((*exp)->braces)
					skip_char_no_copy(exp);
				return (true);
			}
			if (handle_backslash(exp, TOKEN, Q_DOUBLE))
				return (true);
		}
		if (is_closing_quote(CURRENT_CHAR, &quote))
		{
			skip_char_no_copy(exp);
			return (true);
		}
		copy_char_and_increment(exp);
		return (true);
	}
	return (false);
}

// used by token
bool	process_unquoted_sequence(t_exp_mem **exp, t_mem **mem)
{
	if (handle_dollar_sign(exp, mem))
	{
		if ((*exp)->braces)
			skip_char_no_copy(exp);
		return (true);
	}
	if (handle_backslash(exp, TOKEN, Q_OFF))
		return (true);
	return (false);
}
