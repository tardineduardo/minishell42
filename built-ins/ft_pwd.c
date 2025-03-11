/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:28:21 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	**Mandatory: pwd with no options***
	PWD just print the full filename  of the current
	working directory. Strict to the point.

	Implementation: the program iterates throught
	our linked list until it finds PWD and print it without
	variable name PWD or '=' operator.
*/
void	ft_pwd(t_env **ms_env)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		if (ft_strcmp(current->variable, "PWD") == 0)
		{
			ft_printf("%s\n", current->value);
			break ;
		}
		current = current->next;
	}
}