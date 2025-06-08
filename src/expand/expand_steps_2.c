/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_steps_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/06 20:02:14 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/expand.h"

static bool	ft_condition(t_exp_mem **exp, int mode)
{
	if (mode == 1)
	{
		if ((*exp)->raw[(*exp)->a] != '$'
			|| (*exp)->raw[(*exp)->a + 1] == '\0'
			|| ft_isspace((*exp)->raw[(*exp)->a + 1]))
			return (true);
	}
	else if (mode == 2)
	{
		if ((*exp)->raw[(*exp)->a + 1]
			&& (ft_isalnum((*exp)->raw[(*exp)->a + 1])
				|| (*exp)->raw[(*exp)->a + 1] == '?'))
			return (true);
	}
	return (false);
}

static t_exit	remove_var_from_string(char **s, size_t index)
{
	size_t	a;

	a = 0;
	while (ft_isalnum((*s)[index + a + 1]))
		a++;
	ft_strlcpy(&(*s)[index], &(*s)[index + a + 1], ft_strlen(&(*s)[index]));
	return (VAR_NOT_FOUND);
}

t_exit	ft_try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	char	**value;
	t_exit	exit;

	value = &(*exp)->value;
	exit = ft_get_variable_value(&(*exp)->raw[(*exp)->a], value, mem);
	if (exit == VAR_FOUND)
		return (ft_insert_var_in_string(*value, (*exp)->a, exp));
	else if (exit == VAR_NOT_FOUND)
		return (remove_var_from_string(&(*exp)->raw, (*exp)->a));
	else if (exit == BAD_SUBSTITUITION)
		return (BAD_SUBSTITUITION);
	else
		return (ERROR);
}

bool	ft_handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
{
	t_exit	exit;

	if (ft_condition(exp, 1))
		return (false);
	if ((*exp)->raw[(*exp)->a + 1] == '{')
	{
		(*exp)->braces = true;
		ft_skip_char_no_copy(exp);
	}
	if (ft_condition(exp, 2))
	{
		exit = ft_try_to_expand_variable(exp, mem);
		if (!exit)
		{
			(*exp)->exit = exit;
			return (false);
		}
		if (exit == VAR_FOUND)
		{
			ft_copy_value_and_increment(exp);
			return (true);
		}
		return (true);
	}
	return (false);
}

bool	ft_handle_backslash(t_exp_mem **exp, t_mode mode, t_quote quote)
{
	if ((*exp)->raw[(*exp)->a] != '\\' || (*exp)->raw[(*exp)->a + 1] == '\0')
		return (false);
	if (mode == TOKEN && quote == Q_OFF)
		return (ft_skip_slash_copy_next_and_increment(exp));
	if (mode == TOKEN && quote == Q_DOUBLE)
	{
		if (ft_strchr("\\\"\'$", (*exp)->raw[(*exp)->a + 1]))
			return (ft_skip_slash_copy_next_and_increment(exp));
	}
	if (mode == HEREDOC && (*exp)->hd_mode == EXPAND)
	{
		if (ft_strchr("\\$", (*exp)->raw[(*exp)->a + 1]))
			return (ft_skip_slash_copy_next_and_increment(exp));
	}
	return (false);
}
