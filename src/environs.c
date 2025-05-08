/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/08 10:24:09 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/expand.h"
#include "../include/parsing.h"
#include "../include/environs.h"
#include "../include/readline.h"

void	*ft_init_environs(t_env_mem **env, char **envp)
{
	if (!envp)
		return (NULL);
	if (!(*envp))
		return (envp); // I'm just returning a valid pointer. 
	while (*envp != NULL)
	{
		(*env)->result = ft_split_char(*envp, '=');
		if (!(*env)->result)
			return (ft_env_error("Envp split error", env));
		if ((*env)->result[1])
			(*env)->new_node = ft_init_env_node((*env)->result[0], (*env)->result[1], true);
		else
			(*env)->new_node = ft_init_env_node((*env)->result[0], "", false);
		if (!(*env)->new_node)
			return (ft_env_error("Init node error", env));
		if(!ft_add_to_envlist(&(*env)->envlist, (*env)->new_node))
			return (ft_env_error("Add to envlist error", env));
		ft_free_and_null_str_array(&(*env)->result);
		envp++;
	}
	ft_env_readonly(&(*env)->envlist);
	ft_env_block_unset(&(*env)->envlist);
	return ((*env)->envlist);
}


/*
	Need to understand what set of variables are readonly
	All the ones listed below are or readonly or blocked_unset
*/
void	ft_env_readonly(t_list **envlist)
{
	t_list		*trav;
	t_env_node	*current;

	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)(* envlist)->content;
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
		trav = trav->next;
	}
}
/*
	Need to understand what set of variables are 'blocked' to unset
	All the ones listed below are or readonly or blocked_unset
*/
void	ft_env_block_unset(t_list **envlist)
{
	t_list		*trav;
	t_env_node	*current;

	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)(* envlist)->content;
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
		trav = trav->next;
	}
}

//----------- NODES and LISTS ----------------

t_env_node	*ft_init_env_node(char *variable, char *value, bool visible)
{
	t_env_node *new;

	new = malloc(sizeof(t_env_node));
	if (!new)
		return (ft_env_syscall_error("Init node malloc error"));
	new->variable = ft_strdup(variable);
    new->value = ft_strdup(value);
    new->readonly = false;
	new->block_unset = false;
	new->visible = visible;
	return (new);
}

t_list	*ft_add_to_envlist(t_list **envlist, t_env_node *new_node)
{
	t_list *new_envlist_node;

	new_envlist_node = ft_lstnew(new_node);
	if (!(new_envlist_node))
		return (NULL);

	ft_lstadd_back(envlist, new_envlist_node);
	return (*envlist);
}




// --------------- ERRORS --------------------

void	*ft_env_syscall_error(char *message)
{
	ft_dprintf(STDERR_FILENO, "%s: %s [%i]\n", message, strerror(errno), errno);
	return (NULL);
}

void	*ft_env_error(char *message, t_env_mem **env)
{
	ft_dprintf(STDERR_FILENO, "Minishell: %s\n", message);
	ft_lstclear(&(*env)->envlist, ft_del_env_node);
	ft_del_env_node(&(*env)->new_node);
	ft_free_and_null_str_array(&(*env)->result);
	ft_free_and_null((void *)env);
	return (NULL);
}

void ft_del_env_node(void *content)
{
	t_env_node	*node;

	if (!content)
		return ;
	node = (t_env_node *)content;
	ft_free_and_null((void *)&node->variable);
	ft_free_and_null((void *)&node->value);
	ft_free_and_null((void *)&node);
}



void	*ft_init_env_memory(t_mem **mem)
{
	(*mem)->environs = malloc(sizeof(t_env_mem));
	if (!(*mem)->environs)
		return (NULL);
	(*mem)->environs->envlist = NULL;
	(*mem)->environs->new_node = NULL;
	(*mem)->environs->result = NULL;
	return (mem);
}

void	ft_clear_env_mem(t_env_mem **env)
{
	ft_lstclear(&(*env)->envlist, ft_del_env_node);
	ft_free_and_null_str_array(&(*env)->result);
	free(*env);
	return ;
}
