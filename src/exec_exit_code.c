/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit_code.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:03:08 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/14 13:45:17 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/environs.h"

void	ft_ms_env_update_exit_code(t_list **envlist, char *variable, int value)
{
	t_list		*trav;
	t_env_node	*current;

	if (!envlist || !variable)
		return ;
	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, variable) == 0)
		{
			free(current->value);
			current->value = ft_strdup(ft_itoa(value));
			return ;
		}
		trav = trav->next;
	}
	return ;
}

void	ft_ms_env_add_exit_code(t_list **envlist, char *variable, int value)
{
	t_env_node	*new_env_node;

	if (!(*envlist) || !variable)
		return ;
	new_env_node = ft_init_env_node(variable, ft_itoa(value), false);
	if (!ft_add_to_envlist(envlist, new_env_node))
		return ;
}
