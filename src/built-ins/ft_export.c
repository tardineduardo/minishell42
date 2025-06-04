/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:42:45 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/04 19:52:05 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"
#include "../../include/builtins.h"

t_list	*ft_find_var(t_list *envlist, char *var)
{
	t_list		*trav;
	t_env_node	*envnode;

	trav = envlist;
	while (trav)
	{
		envnode = (t_env_node *)trav->content;
		if (ft_strcmp(envnode->variable, var) == 0)
			return (trav);
		trav = trav->next;
	}
	return (NULL);
}

void	ft_update_envnode_value(t_list *var_found, char *val)
{
	t_env_node	*node;

	node = (t_env_node *)var_found->content;
	if (node->value && val[0] != '\0')
	{
		free(node->value);
		node->value = val;
		if (node->visible == false)
			node->visible = true;
	}
}

void	*ft_ms_env_add(t_list **envlist, char *var, char *val)
{
	t_env_node	*new_env_node;
	bool		isvisible;

	if (!(*envlist) || !var || !val)
		return (NULL);
	if (val[0] != '\0')
		isvisible = true;
	else
		isvisible = false;
	new_env_node = ft_init_env_node(var, val, isvisible);
	if (!new_env_node)
		return (NULL);
	if (!ft_add_to_envlist(envlist, new_env_node))
		return (NULL);
	return (*envlist);
}

int	ft_export_list(t_list **envlist)
{
	t_list		*trav;
	t_env_node	*envnode;

	trav = *envlist;
	while (trav)
	{
		envnode = (t_env_node *)trav->content;
		if (ft_strcmp(envnode->variable, "?") != 0)
			ft_dprintf(1, "declare -x %s=\"%s\"\n", envnode->variable,
				envnode->value);
		trav = trav->next;
	}
	return (EXIT_SUCCESS);
}

int	ft_export(t_list **envlist, char *variable_value)
{
	char	*var;
	char	*val;
	t_list	*var_found;

	if (!envlist)
		return (EXIT_FAILURE);
	if (!variable_value)
		return (ft_export_list(envlist));
	if (variable_value[0] == '=')
		return (ft_error_export(variable_value));
	if (!ft_get_var_and_value(variable_value, &var, &val))
		return (EXIT_FAILURE);
	if (!ft_is_valid_varname(var, variable_value))
		return (EXIT_FAILURE);
	var_found = ft_find_var(*envlist, var);
	if (var_found)
	{
		ft_update_envnode_value(var_found, val);
		free(var);
	}
	else
		if (!ft_ms_env_add(envlist, var, val))
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
