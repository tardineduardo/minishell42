/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:00 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 12:04:15 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_unset(t_env **ms_env, char *variable, int fd)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		if (ft_strcmp(current->variable, variable) == 0)
		{
			if (current->readonly == false && current->block_unset == false)
				ft_lstdel_one_node_env(ms_env, variable);
			else if (current->readonly == true)
				ft_dprintf(fd, "Error: cannot unset readonly variable\n");
			return ;
		}
		current = current->next;
	}
}