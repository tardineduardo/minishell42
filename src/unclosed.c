/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:14:49 by eduribei          #+#    #+#             */
/*   Updated: 2025/04/19 15:07:13 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_unclosed(char *s)
{
	t_quote	quote;
	t_quote	prev;
	size_t	a;
	
	a = 0;
	quote = Q_OFF;
	while(s[a])
	{	
		prev = quote;
		update_quote_flag(s, quote, a);

		skip_if_quote_changed();

		if (ft_strchr("{\"\'", s[a]))




	}



}




void	update_quote_flag(char *s, t_quote *quote, int index)
{
	char c;

	c = s[index];
	if (!ft_isquote(c))
		return ;
	if(!is_char_escaped(s, index))
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