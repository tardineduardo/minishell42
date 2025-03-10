/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/10 15:56:46 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*ft_ms_env(char **envp)
{
	t_env	*env_cpy;
	t_env	*new_node;
	char	**result;

	env_cpy = NULL;
	while (*envp != NULL)
	{
		result = ft_split_char(*envp, '=');
		// Nem sempre vai ter alguma informacao depois do '='
		if (result[1])
			new_node = ft_lstnew_env(result[0], result[1]);
		else
			new_node = ft_lstnew_env(result[0], "");
		if (!new_node)
		{
			ft_free_split(result, 2);
			return NULL;
		}
		ft_lstadd_back_env(&env_cpy, new_node);
		ft_free_split(result, 2);
		envp++;
	}
	return (env_cpy);
}

void	ft_ms_env_update(t_env **ms_env, char *variable, char *value)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		if (ft_strcmp(current->variable, variable) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
}

void	ft_ms_env_add(t_env **ms_env, char *variable_value)
{
	char	**result;
	t_env	*new_node;

	result = ft_split_char(variable_value, '=');
	new_node = ft_lstnew_env(result[0], result[1]);
	ft_lstadd_back_env(ms_env, new_node);
	ft_free_split(result, 1);	
}
