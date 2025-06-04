/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_steps_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/expand.h"

// used by delim, token
void	ft_update_quote_flag(char *s, t_quote *quote, int index)
{
	char	c;

	c = s[index];
	if (!ft_isquote(c))
		return ;
	if (!ft_is_char_escaped(s, index))
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
bool	ft_skip_if_quote_changd(t_exp_mem **exp, t_quote *quote, t_quote *prev)
{
	if (*quote != *prev)
	{
		ft_skip_char_no_copy(exp);
		return (true);
	}
	return (false);
}

// used by delim, token
bool	ft_process_inside_sg_quotes(t_exp_mem **exp, t_quote quote)
{
	if (quote == Q_SINGLE)
	{
		while (!ft_is_closing_quote((*exp)->raw[(*exp)->a], &quote))
			ft_copy_char_and_increment(exp);
		return (true);
	}
	return (false);
}

// used by delim, token
bool	ft_process_inside_db_quote(t_exp_mem **exp, t_mem **mem,
			t_quote quote)
{
	t_mode	mode;

	mode = (*exp)->mode;
	if (quote == Q_DOUBLE)
	{
		if (mode == TOKEN)
		{
			if (ft_handle_dollar_sign(exp, mem))
			{
				if ((*exp)->braces)
					ft_skip_char_no_copy(exp);
				return (true);
			}
			if (ft_handle_backslash(exp, TOKEN, Q_DOUBLE))
				return (true);
		}
		if (ft_is_closing_quote((*exp)->raw[(*exp)->a], &quote))
		{
			ft_skip_char_no_copy(exp);
			return (true);
		}
		ft_copy_char_and_increment(exp);
		return (true);
	}
	return (false);
}

// used by token
bool	ft_process_unquoted_sequence(t_exp_mem **exp, t_mem **mem)
{
	if (ft_handle_dollar_sign(exp, mem))
	{
		if ((*exp)->braces)
			ft_skip_char_no_copy(exp);
		return (true);
	}
	if (ft_handle_backslash(exp, TOKEN, Q_OFF))
		return (true);
	return (false);
}
