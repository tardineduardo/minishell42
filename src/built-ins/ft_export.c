/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:42:45 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/25 15:47:25 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"

t_env_node	*ft_init_env_node_expbuiltin(char *variable, char *value, bool visible);
t_list		*ft_add_to_envlist_expbuiltin(t_list **envlist, t_env_node *new_node);
void		*ft_ms_env_add(t_list **envlist, char *var, char *val);
void		ft_ms_env_update_export(t_list **envlist, char *variable, char *value);

int	ft_export(t_list **envlist, char *variable_value)
{
	char	*var;
	char	*val;
	t_list	*var_found;

	if (!envlist)
		return (EXIT_FAILURE);

	if (!variable_value)
		ft_export_list(envlist);

	if(!ft_get_var_and_value(variable_value, &var, &val))
		return (EXIT_FAILURE);

	if (!ft_validate_var_name)
		return (EXIT_FAILURE);

	var_found = ft_find_var(*envlist, var);

	if (var_found)
		ft_update_envnode_value(var_found, val);
	else
		if(!ft_ms_env_add(envlist, var, val))
			return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}

t_list	*ft_find_var(t_list *envlist, char *var)
{
	t_list		*trav;
	t_env_node	*envnode;

	trav = envlist;
	while(trav)
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
	t_env_node *node;

	node = (t_env_node *)var_found->content;
	if (node->value)
		free(node->value);
	node->value = val;
}

void	*ft_ms_env_add(t_list **envlist, char *var, char *val)
{
	t_env_node	*new_env_node;

	if (!(*envlist) || !var || !val)
		return (NULL);

	new_env_node = ft_init_env_node_expbuiltin(var, val, true);
	if (!new_env_node)
		return (NULL);

	if (!ft_add_to_envlist_expbuiltin(envlist, new_env_node))
		return (NULL);

	return (*envlist);
}


t_env_node	*ft_init_env_node_expbuiltin(char *variable, char *value, bool visible)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	new->variable = ft_strdup(variable);
	new->value = ft_strdup(value);
	new->visible = visible;
	return (new);
}

t_list	*ft_add_to_envlist_expbuiltin(t_list **envlist, t_env_node *new_node)
{
	t_list	*new_envlist_node;

	new_envlist_node = ft_lstnew(new_node);
	if (!(new_envlist_node))
		return (NULL);
	ft_lstadd_back(envlist, new_envlist_node);
	return (*envlist);
}



void	ft_export_list(t_list **envlist)
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


int	ft_validate_var_name(char *varname, char *variable_value)
{
	int	a;
	int	len;

	if (!ft_isalpha(varname[0]) && !varname[0] != '_')
	{
		ft_dprintf(STDERR_FILENO, "export: `%s`: not a valid identifier\n",
			variable_value);
		return (EXIT_FAILURE);
	}
	a = 1;
	len = ft_strlen(varname);
	while (a < len)
	{
		if (!ft_isalnum(varname[a]) && varname[a] != '_')
		{
			ft_dprintf(STDERR_FILENO, "export: `%s`: not a valid identifier\n",
				variable_value);
			return (EXIT_FAILURE);
		}
		a++;
	}
	return (EXIT_SUCCESS);
}