/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_validation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:10:02 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/24 12:14:55 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/readline.h"

bool	is_char_escaped_rdl(char *s, int index)
{
	int	count;

	count = 0;
	if (index <= 0)
		return (false);
	index--;
	while (index >= 0 && s[index] == '\\')
	{
		count++;
		index--;
	}
	if (count % 2 == 1)
		return (true);
	return (false);
}

void	update_quote_flag_rdl(char c, t_quote *quote, int index, char *s)
{
	if ((c == '\'' || c == '\"') && !is_char_escaped_rdl(s, index))
	{
		if (*quote == Q_OFF)
		{
			if (c == '\'')
				*quote = Q_SINGLE;
			else if (c == '\"')
				*quote = Q_DOUBLE;
		}
		else if (c == '\'' && *quote == Q_SINGLE)
			*quote = Q_OFF;
		else if (c == '\"' && *quote == Q_DOUBLE)
			*quote = Q_OFF;
	}
	return ;
}

bool	has_unclosed_quotes(char *s)
{
	int		a;
	t_quote	quote;

	a = 0;
	quote = Q_OFF;
	while (s[a])
	{
		update_quote_flag_rdl(s[a], &quote, a, s);
		a++;
	}
	if (quote != Q_OFF)
		return (true);
	return (false);
}
