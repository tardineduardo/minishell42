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
#include "../../include/expand.h"

void	ft_copy_value_and_increment(t_exp_mem **exp)
{
	size_t	len;

	len = ft_strlen((*exp)->value);
	while (len > 0)
	{
		ft_copy_char_and_increment(exp);
		len--;
	}
	ft_free_and_null((void *)&(*exp)->value);
	return ;
}

void	ft_copy_char_and_increment(t_exp_mem **exp)
{
	if ((*exp)->raw[(*exp)->a])
	{
		(*exp)->new[(*exp)->b] = (*exp)->raw[(*exp)->a];
		(*exp)->b++;
		(*exp)->a++;
	}
	return ;
}

void	ft_skip_char_no_copy(t_exp_mem **exp)
{
	(*exp)->a++;
	return ;
}

bool	ft_skip_slash_copy_next_and_increment(t_exp_mem **exp)
{
	(*exp)->a++;
	if ((*exp)->raw[(*exp)->a])
	{
		(*exp)->new[(*exp)->b] = (*exp)->raw[(*exp)->a];
		(*exp)->a++;
		(*exp)->b++;
	}
	return (true);
}

bool	ft_copy_char_copy_next_and_increment(t_exp_mem **exp)
{
	if ((*exp)->raw[(*exp)->a])
	{
		(*exp)->new[(*exp)->b] = (*exp)->raw[(*exp)->a];
		(*exp)->a++;
		(*exp)->b++;
	}
	if ((*exp)->raw[(*exp)->a])
	{
		(*exp)->new[(*exp)->b] = (*exp)->raw[(*exp)->a];
		(*exp)->a++;
		(*exp)->b++;
	}
	return (true);
}
