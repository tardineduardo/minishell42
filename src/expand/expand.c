/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/expand.h"

static void	*copy_to_new_str_delim_mode(t_exp_mem **exp)
{
	t_quote	quote;
	t_quote	prev;
	int		*a;
	char	*s;

	a = &(*exp)->a;
	s = (*exp)->raw;
	quote = Q_OFF;
	while ((*exp)->raw[(*exp)->a])
	{
		if (ft_isquote(s[*a]) && !ft_is_char_escaped(s, *a))
			(*exp)->hd_mode = QUOTED;
		prev = quote;
		ft_update_quote_flag((*exp)->raw, &quote, (*exp)->a);
		if (ft_skip_if_quote_changd(exp, &quote, &prev))
			continue ;
		ft_copy_char_and_increment(exp);
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}

static void	*copy_to_new_str_heredoc_mode(t_exp_mem **exp, t_mem **mem)
{
	while ((*exp)->raw[(*exp)->a])
	{
		if ((*exp)->error)
			return (NULL);
		if ((*exp)->hd_mode == EXPAND)
		{
			if (ft_handle_dollar_sign(exp, mem))
				continue ;
			if (ft_handle_backslash(exp, HEREDOC, Q_NULL))
				continue ;
		}
		ft_copy_char_and_increment(exp);
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}

static void	*copy_to_new_str_token_mode(t_exp_mem **exp, t_mem **mem)
{
	t_quote	quote;
	t_quote	prev;

	quote = Q_OFF;
	while ((*exp)->raw[(*exp)->a])
	{
		if ((*exp)->error)
			return (NULL);
		prev = quote;
		ft_update_quote_flag((*exp)->raw, &quote, (*exp)->a);
		if (ft_skip_if_quote_changd(exp, &quote, &prev))
			continue ;
		if (ft_process_inside_sg_quotes(exp, quote))
			continue ;
		if (ft_process_inside_db_quote(exp, mem, quote))
			continue ;
		if (ft_process_unquoted_sequence(exp, mem))
			continue ;
		ft_copy_char_and_increment(exp);
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}

static void	*start_expansion_for_mode(t_exp_mem **exp, t_mem **mem, t_mode mode)
{
	if (mode == TOKEN)
		return (copy_to_new_str_token_mode(exp, mem));
	if (mode == DELIMITER)
		return (copy_to_new_str_delim_mode(exp));
	if (mode == HEREDOC)
		return (copy_to_new_str_heredoc_mode(exp, mem));
	return (NULL);
}

// return NULL = erro. O tipo de erro fica registrado em exp->exit
char	*ft_expand(char **string, t_mode mode, t_mem **mem)
{
	t_exp_mem	*exp;

	if (!*string)
		return (NULL);
	if (!ft_strlen(*string))
		return (ft_strdup(""));
	exp = (*mem)->expand;
	exp->mode = mode;
	exp->raw = ft_strdup(*string);
	exp->new = ft_calloc(ft_strlen(*string) + 1, sizeof(char));
	exp->alloct = (ft_strlen(*string) + 1) * sizeof(char);
	if (!exp->new)
		return (NULL);
	if (!start_expansion_for_mode(&exp, mem, mode))
		return (NULL);
	exp->temp = *string;
	*string = ft_strdup(exp->new);
	ft_free_and_null((void *)&exp->temp);
	if (!*string)
	{
		ft_reset_parameters(mem);
		return (NULL);
	}
	ft_reset_parameters(mem);
	return (*string);
}
