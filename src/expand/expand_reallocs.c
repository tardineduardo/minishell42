/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_reallocs.c                                  :+:      :+:    :+:   */
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

t_exit	insert_var_in_string(char *insert, size_t index, t_exp_mem **exp)
{
	char	*prefix;
	char	*suffix;
	char	*temp;
	size_t	len;

	len = varlen(&CURRENT_CHAR, (*exp)->braces);
	prefix = ft_substr((*exp)->raw, 0, index);
	suffix = ft_strdup(&CURRENT_CHAR + len + 1);
	if (!prefix || !suffix)
		return (ERROR);
	temp = ft_concatenate(prefix, insert, suffix);
	if (ft_strlen(temp) + 1 > (*exp)->alloct)
	{
		(*exp)->alloct = 2 * (ft_strlen((*exp)->raw) + 1 + ft_strlen(temp) + 1);
		ft_realloc_string(&(*exp)->new, (*exp)-> alloct);
	}
	if (!temp || !(*exp)->raw)
		return (ERROR);
	free(suffix);
	free(prefix);
	ft_free_and_null((void *)&(*exp)->raw);
	(*exp)->raw = temp;
	return (VAR_FOUND);
}

t_exit	remove_var_from_string(char **s, size_t index)
{
	size_t	a;

	a = 0;
	while (ft_isalnum((*s)[index + a + 1]))
		a++;
	ft_strlcpy(&(*s)[index], &(*s)[index + a + 1], ft_strlen(&(*s)[index]));
	return (VAR_NOT_FOUND);
}

size_t	varlen(char *s, bool braces)
{
	size_t	i;

	if (*s == '$')
		s++;
	if (*s == '{')
		s++;
	i = 0;
	if (braces == false)
	{
		if (s[0] == '?')
			return (1);
		while (ft_isalnum(s[i]))
			i++;
	}
	else if (braces == true)
		while ((s[i] != '}'))
			i++;
	return (i);
}
