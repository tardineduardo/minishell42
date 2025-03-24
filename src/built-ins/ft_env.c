/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:20 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:27:56 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
	**Mandatory: env with no options or with no arguments**
	Implementation: iterate through our linked list with all
	environment variables and print the pair VAR_NAME=VALUE.
*/
void	ft_env(t_list *envlist)
{
	t_list		*trav;
	t_env_node	*current;

	trav = envlist;
	
	while (trav)
	{
		current = (t_env_node *)trav->content;
		ft_dprintf(1, "%s=%s\n", current->variable, current->value);
		trav = trav->next;
	}
}