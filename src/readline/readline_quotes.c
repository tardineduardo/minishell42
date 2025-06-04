/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:43:01 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/04 18:14:50 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/readline.h"

bool	ft_is_char_escaped_rdl(char *s, int index)
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

void	ft_update_quote_flag_rdl(char c, t_quote *quote, int index, char *s)
{
	if ((c == '\'' || c == '\"') && !ft_is_char_escaped_rdl(s, index))
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

bool	ft_has_unclosed_quotes(char *s)
{
	int		a;
	t_quote	quote;

	a = 0;
	quote = Q_OFF;
	while (s[a])
	{
		ft_update_quote_flag_rdl(s[a], &quote, a, s);
		a++;
	}
	if (quote != Q_OFF)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error: unclosed quotes.\n");
		return (true);
	}
	return (false);
}
