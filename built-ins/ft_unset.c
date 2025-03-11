/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:00 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:34:29 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	**Mandatory: unset with no options**
	One case accpeted: unset VAR_NAME
		exclude the variable if it is not readonly

	Implementation: iterate through our linked list with
	all environment variables, if find it check if it is 
	readonly and if it is 'blocked' to unset (PWD case). In
	the case it is readonly, we need to print a msg warning 
	the user and no change is made, but in the case that it
	is blocked to unset, just doesnt show anything and doesnt 
	unset.
*/
void	ft_unset(t_env **ms_env, char *variable)
{
	t_env	*current;

	current = *ms_env;
	if (ft_strchr(variable, '=') != NULL)
		return ;
	while (current)
	{
		if (ft_strcmp(current->variable, variable) == 0)
		{
			if (current->readonly == false && current->block_unset == false)
				ft_lstdel_one_node_env(ms_env, variable);
			else if (current->readonly == true)
				ft_printf("Error: cannot unset readonly variable\n");
			return ;
		}
		current = current->next;
	}
}