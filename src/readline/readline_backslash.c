/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_backslash.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:39:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/04 18:09:10 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/readline.h"

static bool	ft_print_and_return_true(void)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error: '\\' at the end.\n");
	return (true);
}

bool	ft_ends_with_backslash(char *s)
{
	int	len;

	if (!s)
		return (false);
	len = ft_strlen(s);
	if (len == 0)
		return (false);
	else if (s[len - 1] == '\\' && !ft_is_char_escaped_rdl(s, len - 1))
		return (ft_print_and_return_true());
	else if (s[len - 1] == '\\' && s[len - 2] != '\\')
		return (ft_print_and_return_true());
	return (false);
}
