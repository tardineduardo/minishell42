/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:00 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/07 12:54:46 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"  
#include "../../include/heredoc.h"
#include "../../include/expand.h"
#include "../../include/tokenize.h"
#include "../../include/environs.h"
#include "../../include/heredoc.h"
#include "../../include/builtins.h"

int	ft_unset(t_list **envlist, char *variable)
{
	t_list		*trav;
	t_env_node	*current;

	trav = *envlist;
	if (variable == NULL)
		return (0);
	if (ft_strchr(variable, '=') != NULL)
		return (-1);
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, variable) == 0)
		{
			ft_lst_destroy_node(envlist, trav, ft_del_env_node);
			break ;
		}
		trav = trav->next;
	}
	return (0);
}
