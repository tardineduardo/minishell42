/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ms_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/06 14:14:19 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
