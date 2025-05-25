/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ops.c                                       :+:      :+:    :+:   */
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

void	copy_value_and_increment(t_exp_mem **exp)
{
	size_t	len;

	len = ft_strlen((*exp)->value);
	while (len > 0)
	{
		copy_char_and_increment(exp);
		len--;
	}
	ft_free_and_null((void *)&(*exp)->value);
	return ;
}

void	copy_char_and_increment(t_exp_mem **exp)
{
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->b++;
		(*exp)->a++;
	}
	return ;
}

void	skip_char_no_copy(t_exp_mem **exp)
{
	(*exp)->a++;
	return ;
}

bool	skip_slash_copy_next_and_increment(t_exp_mem **exp)
{
	(*exp)->a++;
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->a++;
		(*exp)->b++;
	}
	return (true);
}

bool	copy_char_copy_next_and_increment(t_exp_mem **exp)
{
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->a++;
		(*exp)->b++;
	}
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->a++;
		(*exp)->b++;
	}
	return (true);
}
