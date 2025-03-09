/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/09 15:54:14 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	ft_ms_env_item_index(char *ms_env[], char *variable)
// {
// 	int		i;
// 	int		item_index;
// 	size_t	size_variable;

// 	i = 0;
// 	item_index = -1;
// 	size_variable = ft_strlen(variable);
// 	while (ms_env[i])
// 	{
// 		if (ft_strncmp(ms_env[i], variable, size_variable) == 0)
// 		{
// 			item_index = i;
// 			break ;
// 		}
// 		i++;
// 	}
// 	return (item_index);
// }

char	**ft_ms_env(char *envp[])
{
	size_t	size_item;
	char	**env_cpy;
	int		env_size;
	int		i;
	int		j;
	
	env_size = 0;
	while (envp[env_size] != NULL)
		env_size++;
	env_cpy = ft_calloc(sizeof(char *), (env_size + 1));
	if (!env_cpy)
		return (NULL);
	i = 0;
	while (i < env_size)
	{
		size_item = ft_strlen(envp[i]);
		env_cpy[i] = ft_calloc(sizeof(char), (size_item + 1));
		if (!env_cpy[i])
		{
			while (--i >= 0)
				free(env_cpy[i]);
			free(env_cpy);
			return (NULL);
		}
		j = 0;
		while (envp[i][j] != '\0')
		{
			env_cpy[i][j] = envp[i][j];
			j++;
		}
		i++;
	}
	return (env_cpy);
}

t_env	*ft_ms_env(char **envp)
{
	t_env	*env_cpy;
	t_env	*new_node;
	char	*variable;
	char	*value;
	int		readonly;

	env_cpy = NULL;
	new_node = ft_lstnew(variable, value, readonly);
	ft_lstadd_back(env_cpy, new_node);
	return (env_cpy);
}

// void	ft_ms_env_update(char *env[], char *variable, char *new_value)
// {
// 	char	*first_join;
// 	char	*variable_value;
// 	int		item_index;
// 	int		i;
	
// 	i = 0;
// 	item_index = ft_ms_env_item_index(env, variable);
// 	first_join = ft_strjoin(variable, "=");
// 	variable_value = ft_strjoin(first_join, new_value);
// 	env[item_index] = variable_value;
// }

// void	ft_ms_env_add(char *env[], char *new_variable_value)
// {
// 	size_t	size_item;
// 	char	**env_cpy;
// 	int		env_size;
// 	int		i;
// 	int		j;
	
// 	env_size = 0;
// 	while (env[env_size] != NULL)
// 		env_size++;
// 	env_size++;
// 	env_cpy = ft_calloc(sizeof(char *), (env_size + 1));
// 	if (!env_cpy)
// 		return ;
// 	i = 0;
// 	while (i < env_size)
// 	{
// 		if (i == (env_size - 1))
// 			size_item = ft_strlen(new_variable_value);
// 		else if (i < (env_size - 1))
// 			size_item = ft_strlen(env[i]);
// 		env_cpy[i] = ft_calloc(sizeof(char), (size_item + 1));
// 		if (!env_cpy[i])
// 		{
// 			while (--i >= 0)
// 				free(env_cpy[i]);
// 			free(env_cpy);
// 			return ;
// 		}
// 		j = 0;
// 		if (i == (env_size - 1))
// 		{
// 			while (new_variable_value[j] != '\0')
// 			{
// 				env_cpy[i][j] = new_variable_value[j];
// 				j++;
// 			}
// 		}
// 		else if (i < (env_size - 1))
// 		{
// 			while (env[i][j] != '\0')
// 			{
// 				env_cpy[i][j] = env[i][j];
// 				j++;
// 			}
// 		}
// 		i++;
// 	}
	// this part is working well, the problem resides in the fact that after creating a new ms_env, other parts dont "know" that...
	// here i just free the one that the other programs have access and when I call it from any other built-in it crashes
	// env_size--;
	// while (--env_size >= 0)
	// 	free(env[env_size]);
	// free(env);
}
