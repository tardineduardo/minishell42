/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:20 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/14 13:28:02 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"  
#include "../../include/heredoc.h"
#include "../../include/expand.h"
#include "../../include/tokenize.h"
#include "../../include/environs.h"
#include "../../include/heredoc.h"
#include "../../include/builtins.h"

/*
	**Mandatory: env with no options or with no arguments**
	Implementation: iterate through our linked list with all
	environment variables and print the pair VAR_NAME=VALUE.
*/
int	ft_env(t_list *envlist)
{
	t_list		*trav;
	t_env_node	*current;

	trav = envlist;
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (current->visible == true)
			ft_dprintf(1, "%s=%s\n", current->variable, current->value);
		trav = trav->next;
	}
	return (0);
}
