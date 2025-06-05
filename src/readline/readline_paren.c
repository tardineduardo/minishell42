/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_paren.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:39:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/05 15:26:54 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/readline.h"

static bool	ft_print_and_return_true(void)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error: unclosed parenthesis.\n");
	return (true);
}

bool	ft_has_unclosed_paren(char *s)
{
	int		open_count;
	int		i;
	t_quote	quote;

	open_count = 0;
	i = 0;
	quote = Q_OFF;
	while (s[i])
	{
		ft_update_quote_flag_rdl(s[i], &quote, i, s);
		if (quote != Q_SINGLE)
		{
			if (s[i] == '(' && !ft_is_char_escaped_rdl(s, i))
				open_count++;
			else if (s[i] == ')' && !ft_is_char_escaped_rdl(s, i))
			{
				if (open_count > 0)
					open_count--;
			}
		}
		i++;
	}
	if (open_count != 0)
		return (ft_print_and_return_true());
	return (false);
}
