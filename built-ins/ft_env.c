/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:20 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:27:56 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	**Mandatory: env with no options or with no arguments**
	Implementation: iterate through our linked list with all
	environment variables and print the pair VAR_NAME=VALUE.
*/
void	ft_env(t_env **ms_env)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		// TODO: ft_printf and ft_dprintf from edu is not working properly, in the case below it prints twice the first arg.
		ft_printf("%s=%s\n", current->variable, current->value);
		current = current->next;
	}
}