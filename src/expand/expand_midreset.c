/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_midreset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/expand.h"

void	ft_reset_parameters(t_mem **mem)
{
	t_exp_mem	*exp;

	exp = (*mem)->expand;
	exp->a = 0;
	exp->b = 0;
	ft_free_and_null((void *)&exp->new);
	ft_free_and_null((void *)&exp->raw);
	ft_free_and_null((void *)&exp->value);
	ft_free_and_null((void *)&exp->sortedvars);
	exp->exit = INIT;
	exp->error = false;
	exp->braces = false;
}

void	*ft_init_exp_memory(t_mem **mem)
{
	(*mem)->expand = malloc(sizeof(t_exp_mem));
	if (!(*mem)->expand)
		return (NULL);
	(*mem)->expand->a = 0;
	(*mem)->expand->b = 0;
	(*mem)->expand->braces = false;
	(*mem)->expand->alloct = 0;
	(*mem)->expand->hd_mode = EXPAND;
	(*mem)->expand->raw = NULL;
	(*mem)->expand->new = NULL;
	(*mem)->expand->value = NULL;
	(*mem)->expand->error = false;
	(*mem)->expand->exit = INIT;
	(*mem)->expand->sortedvars = NULL;
	return ((*mem)->expand);
}

void	ft_clear_exp_mem(t_exp_mem **exp)
{
	ft_free_and_null((void *)&(*exp)->new);
	free(*exp);
	return ;
}
