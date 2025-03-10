/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/10 16:19:49 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_free_split(char **result, int i)
{
	while (i > 0)
		free(result[--i]);
	free(result);
	return (NULL);
}

void	ft_pwd(t_env **ms_env, int fd)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		if (ft_strcmp(current->variable, "PWD") == 0)
		{
			ft_dprintf(fd, "%s\n", current->value);
			break ;
		}
		current = current->next;
	}
}