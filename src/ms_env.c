/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/20 21:28:11 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
	Need to understand what set of variables are readonly
	All the ones listed below are or readonly or blocked_unset
*/
static void	ft_ms_env_readonly(t_env **ms_env)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		if (ft_strcmp(current->variable, "PWD") == 0)
			current->readonly = true;
		else if (ft_strcmp(current->variable, "OLDPWD") == 0)
			current->readonly = true;
		else if (ft_strcmp(current->variable, "HOME") == 0)
			current->readonly = true;
		else if (ft_strcmp(current->variable, "SHELL") == 0)
			current->readonly = true;
		else if (ft_strcmp(current->variable, "IFS") == 0)
			current->readonly = true;
		else if (ft_strcmp(current->variable, "PATH") == 0)
			current->readonly = true;
		else if (ft_strcmp(current->variable, "UID") == 0)
			current->readonly = true;
		else if (ft_strcmp(current->variable, "PPID") == 0)
			current->readonly = true;
		current = current->next;
	}
}
/*
	Need to understand what set of variables are 'blocked' to unset
	All the ones listed below are or readonly or blocked_unset
*/
static void	ft_ms_env_block_unset(t_env **ms_env)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		if (ft_strcmp(current->variable, "PWD") == 0)
			current->block_unset = true;
		else if (ft_strcmp(current->variable, "OLDPWD") == 0)
			current->block_unset = true;
		else if (ft_strcmp(current->variable, "HOME") == 0)
			current->block_unset = true;
		else if (ft_strcmp(current->variable, "SHELL") == 0)
			current->block_unset = true;
		else if (ft_strcmp(current->variable, "IFS") == 0)
			current->block_unset = true;
		else if (ft_strcmp(current->variable, "PATH") == 0)
			current->block_unset = true;
		else if (ft_strcmp(current->variable, "UID") == 0)
			current->block_unset = true;
		else if (ft_strcmp(current->variable, "PPID") == 0)
			current->block_unset = true;
		current = current->next;
	}
}
/*
	update all nodes from our recently created linked list 
	with the real values of readonly and blocked_unset bools
	values
*/
static void	ft_ms_env_update_bools(t_env **ms_env)
{
	if (!ms_env)
		return ;
	ft_ms_env_readonly(ms_env);
	ft_ms_env_block_unset(ms_env);
}

/*
	We receive the array of pointers of chars with all
	the environment variables from main ft when start 
	the program and save it on a linked list where each
	node has specific infos necessary to handle this list.
	(vide Struct t_env).
*/
t_env	*ft_ms_env(char **envp)
{
	t_env	*env_cpy;
	t_env	*new_node;
	char	**result;

	if (!envp)
		return (NULL);
	env_cpy = NULL;
	while (*envp != NULL)
	{
		result = ft_split_char(*envp, '=');
		// Nem sempre vai ter alguma informacao depois do '='
		if (result[1])
			new_node = ft_lstnew_env(result[0], result[1]);
		else
			new_node = ft_lstnew_env(result[0], "");
		if (!new_node)
		{
			ft_free_split(result, 2);
			return NULL;
		}
		ft_lstadd_back_env(&env_cpy, new_node);
		ft_free_split(result, 2);
		envp++;
	}
	ft_ms_env_update_bools(&env_cpy);
	return (env_cpy);
}

/*
	search inside our linked list with all env vars one specific and 
	when finds it update the value also passed as a parameter.
*/
void	ft_ms_env_update(t_env **ms_env, char *variable, char *value)
{
	t_env	*current;

	if (!ms_env || !variable || !value)
		return ;
	current = *ms_env;
	while (current)
	{
		if (ft_strcmp(current->variable, variable) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
}

/*
	Add to the end of the linked list a new node with a variable_value.
	Inside the ft, we manage to split the variable and save it properly
	in a node.

	QUESTION: is it better to receive one unique variable_value parameter
	or reveice them separated? the responsability is from the tokenization
	or is from the ms_env_add?
*/
void	ft_ms_env_add(t_env **ms_env, char *variable_value)
{
	char	**result;
	t_env	*new_node;

	if (!ms_env || !variable_value)
		return ;
	result = ft_split_char(variable_value, '=');
	new_node = ft_lstnew_env(result[0], result[1]);
	ft_lstadd_back_env(ms_env, new_node);
	ft_free_split(result, 1);	
}




t_env	*ft_lstnew_env(char *variable, char *value)
{
	t_env	*a;

	a = malloc(sizeof(t_env));
	if (!a)
		return (NULL);
	a->variable = ft_strdup(variable);
    a->value = ft_strdup(value);
    a->readonly = false;
	a->block_unset = false;
	a->next = NULL;
	return (a);
}

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*trav;

	if (lst == NULL)
		return (NULL);
	trav = lst;
	while (trav->next != NULL)
		trav = trav->next;
	return (trav);
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		temp = ft_lstlast_env(*lst);
		temp->next = new;
	}
}

void	ft_lstdel_one_node_env(t_env **lst, char *variable)
{
	t_env	*current;
	t_env	*prev;

	if (lst == NULL || *lst == NULL || variable == NULL)
		return ;
	current = *lst;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->variable, variable) == 0)
		{
			if (prev == NULL)
				*lst = current->next;
			else
				prev->next = current->next;
			free(current->variable);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}