/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:42:45 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/10 21:22:13 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_export(t_env **ms_env, char *variable_value, int fd)
{
	char	**result;
	t_env	*current;

	(void)fd;
	current = *ms_env;
	result = ft_split_char(variable_value, '=');
	while (current)
	{
		if (ft_strcmp(current->variable, result[0]) == 0)
		{
			ft_ms_env_update(ms_env, result[0], result[1]);
			ft_free_split(result, 2);
			return ;
		}
		current = current->next;
	}
	ft_free_split(result, 2);
	ft_ms_env_add(ms_env, variable_value);
}