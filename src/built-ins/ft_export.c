/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:42:45 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/14 13:35:17 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"

/*
	**Mandatory: export with no options**
	Cases possible: 
		1. export VAR_NAME=VALUE
			a. If the VAR_NAME already exists, checks if it is readonly and
			after that updates the value. If readonly, nothing happens even 
			a warning msg;
			b. If the VAR_NAME does not exists, append the KEY=VALUE to our
			linked list with all other environment variables
		2. export
			Passing only the command export, we need to print a list of all 
			the exported variables TODO: how to identify if a env var is 
			exported or not? running the command on real bash some vars
			appears that are not on the call of env. Create a bool inside t_env
			to mark with it is exported or not?
*/
t_env_node	*ft_init_env_node_expbuiltin(char *variable, char *value, bool visible);
t_list		*ft_add_to_envlist_expbuiltin(t_list **envlist, t_env_node *new_node);

void		ft_ms_env_add(t_list **envlist, char *variable_value);
void		ft_ms_env_update_export(t_list **envlist, char *variable, char *value);
//@luiscarvalhofrade essa lógica a gente preisa rever depois, pois
//podemos passar uma variável vazia, tipo "NOVA_VAR="

//nomes de variáveis em Bash devem começar com uma letra ou underline (_) — 
// não podem começar com um número. validar isso.
int	ft_export(t_list **envlist, char *variable_value)
{
	int			i;
	char		**result;
	t_list		*trav;
	t_env_node	*current;

	if (!envlist)
		return (1);
	if (!variable_value)
	{
		trav = *envlist;
		while (trav)
		{
			current = (t_env_node *)trav->content;
			if (current->visible)
				ft_dprintf(1, "declare -x %s=\"%s\"\n", current->variable, current->value);
			trav = trav->next;
		}
		return (0);
	}
	if (ft_strcmp(variable_value, "=") == 0)
	{
		ft_dprintf(2, "export: `%s`: not a valid identifier\n", variable_value);
		return (1);
	}
	result = ft_split_char(variable_value, '=');
	if (!result || !result[0])
		return (1);
	if (!ft_isalpha(result[0][0]) && result[0][0] != '_')
	{
		ft_free_split(result, 2);
		ft_dprintf(2, "export: `%s`: not a valid identifier\n", variable_value);
		return (1);
	}
	i = 1;
	while (result[0][i])
	{
		if (!ft_isalnum(result[0][i]) && result[0][i] != '_')
		{
			ft_free_split(result, 2);
			ft_dprintf(2, "export: `%s`: not a valid identifier\n", variable_value);
			return (1);
		}
		i++;
	}
	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, result[0]) == 0)
		{
			if (!current->readonly && !current->block_unset)
			{
				free(current->value);
				if (result[1] != NULL)
					current->value = ft_strdup(result[1]);
				else
					current->value = ft_strdup("");
				current->visible = true;
			}
			ft_free_split(result, 2);
			return (0);
		}
		trav = trav->next;
	}
	ft_ms_env_add(envlist, variable_value);
	ft_free_split(result, 2);
	return (0);
}

void	ft_ms_env_add(t_list **envlist, char *variable_value)
{
	char		**result;
	t_env_node	*new_env_node;

	if (!(*envlist) || !variable_value)
		return ;
	result = ft_split_char(variable_value, '=');
	if (ft_strchr(variable_value, '=') == NULL)
		new_env_node = ft_init_env_node_expbuiltin(result[0], " ", false);
	else
		new_env_node = ft_init_env_node_expbuiltin(result[0], result[1], true);
	if (!ft_add_to_envlist_expbuiltin(envlist, new_env_node))
		ft_free_and_null_str_array(&result);
	return ;
}

void	ft_ms_env_update_export(t_list **envlist, char *variable, char *value)
{
	t_list		*trav;
	t_env_node	*current;

	if (!envlist || !variable || !value)
		return ;
	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, variable) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		trav = trav->next;
	}
	return ;
}

// @luiscarvalhofrade criei essas duas funções abaixo como duplicadas do
// arquivo environ.c. eu acho melhor no começo a gente isolar as funções
// para evitar mudar uma coisa em um lugar e quebrar em outro. no fim do
// projeto, a gente pode criar uma lista de shared functions.   
t_env_node	*ft_init_env_node_expbuiltin(char *variable, char *value, bool visible)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	new->variable = ft_strdup(variable);
	new->value = ft_strdup(value);
	new->readonly = false;
	new->block_unset = false;
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
