/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_valids.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/expand.h"

bool	ft_is_closing_quote(char c, t_quote *quote)
{
	if (c == '\'' && *quote == Q_SINGLE)
	{
		*quote = Q_OFF;
		return (true);
	}
	else if (c == '\"' && *quote == Q_DOUBLE)
	{
		*quote = Q_OFF;
		return (true);
	}
	return (false);
}

bool	ft_is_char_escaped(char *string, int a)
{
	int	escapecount;

	if (a <= 0)
		return (false);
	escapecount = 0;
	while (a > 0)
	{
		if (string[a - 1] == '\\')
			escapecount++;
		if (escapecount % 2 == 1)
			return (true);
		a--;
	}
	return (false);
}

t_delim	ft_is_delim_normal_or_quoted(char *s)
{
	int	a;
	int	escapecount;

	a = 0;
	while (s[a])
	{
		if (a == 0 && (ft_isquote(s[a])))
			return (EXPAND);
		if (ft_isquote(s[a]))
		{
			escapecount = 0;
			while (a > 0 && s[a - 1 - escapecount] == '\\')
				escapecount++;
			if (escapecount % 2 == 0)
				return (QUOTED);
		}
		a++;
	}
	return (QUOTED);
}
