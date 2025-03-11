/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:42:45 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:28:08 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


/*
	**Mandatory: export with no options**
	Cases possible: 
		1. export VAR_NAME=VALUE
			a. If the VAR_NAME already exists, checks if it is readonly and
			after that updates the value. If readonly, nothing happens even 
			a warning msg;
			b. If the VAR_NAME does not exists, append the KEY=VALUE to our
			linked list with all other environment variables
		2. export
			Passing only the command export, we need to print a list of all 
			the exported variables TODO: how to identify if a env var is 
			exported or not? running the command on real bash some vars
			appears that are not on the call of env. Create a bool inside t_env
			to mark with it is exported or not?
*/
void	ft_export(t_env **ms_env, char *variable_value)
{
	char	**result;
	t_env	*current;

	current = *ms_env;
	result = ft_split_char(variable_value, '=');
	while (current)
	{
		if (ft_strcmp(current->variable, result[0]) == 0)
		{
			if (current->block_unset == false && current->readonly == false)
			{
				ft_ms_env_update(ms_env, result[0], result[1]);
				ft_free_split(result, 2);
				return ;
			}
		}
		current = current->next;
	}
	ft_free_split(result, 2);
	ft_ms_env_add(ms_env, variable_value);
}
