/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:42:45 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:28:08 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


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
t_env_node	*ft_init_env_node_expbuiltin(char *variable, char *value);
t_list	*ft_add_to_envlist_expbuiltin(t_list **envlist, t_env_node *new_node);



//@luiscarvalhofrade essa lógica a gente preisa rever depois, pois
//podemos passar uma variável vazia, tipo "NOVA_VAR="

//nomes de variáveis em Bash devem começar com uma letra ou underline (_) — 
// não podem começar com um número. validar isso.
void	ft_export(t_list **envlist, char *variable_value)
{
	char	**result;
	t_list	*trav;
	t_env_node	*current;

	trav = *envlist;
	result = ft_split_char(variable_value, '=');
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, result[0]) == 0)
		{
			if (current->block_unset == false && current->readonly == false)
			{
				ft_ms_env_update_export(envlist, result[0], result[1]);
				ft_free_split(result, 2);
				return ;
			}
		}
		trav = trav->next;
	}
	ft_free_and_null_str_array(&result);
	ft_ms_env_add(envlist, variable_value);
	return ;
}

void	ft_ms_env_add(t_list **envlist, char *variable_value)
{
	char		**result;
	t_env_node	*new_env_node;

	if (!(*envlist) || !variable_value)
		return ;
	result = ft_split_char(variable_value, '=');
		
	new_env_node = ft_init_env_node_expbuiltin(result[0], result[1]);
	if(!ft_add_to_envlist_expbuiltin(envlist, new_env_node))

	ft_free_and_null_str_array(&result);
	return ;
}


void	ft_ms_env_update_export(t_list **envlist, char *variable, char *value)
{
	t_list *trav;
	t_env_node *current;

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
t_env_node	*ft_init_env_node_expbuiltin(char *variable, char *value)
{
	t_env_node *new;

	new = malloc(sizeof(t_env_node));
	//if (!new)
		//return (ft_env_syscall_error("Init node malloc error"));
	new->variable = ft_strdup(variable);
    new->value = ft_strdup(value);
    new->readonly = false;
	new->block_unset = false;
	return (new);
}

t_list	*ft_add_to_envlist_expbuiltin(t_list **envlist, t_env_node *new_node)
{
	t_list *new_envlist_node;

	new_envlist_node = ft_lstnew(new_node);
	if (!(new_envlist_node))
		return (NULL);

	ft_lstadd_back(envlist, new_envlist_node);
	return (*envlist);
}