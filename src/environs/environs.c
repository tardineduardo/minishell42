/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/25 18:29:53 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/expand.h"
#include "../../include/parsing.h"
#include "../../include/environs.h"
#include "../../include/readline.h"
#include "../../include/execution.h"

void	*ft_init_environs(t_env_mem **env, char **envp)
{
	char *var;
	char *val;

	var = NULL;
	val = NULL;
	if (!envp)
		return (NULL);
	if (!(*envp))
		return (envp);
	while (*envp != NULL)
	{
		if (!ft_get_var_and_value(*envp, &var, &val))
			return(NULL);
		(*env)->new_node = ft_init_env_node(var, val, true);
		if (!(*env)->new_node)
			return (NULL);
		if (!ft_add_to_envlist(&(*env)->envlist, (*env)->new_node))
			return (NULL);
		envp++;
	}
	return ((*env)->envlist);
}

void	*ft_get_var_and_value(char *envp, char **var, char **val)
{
	char *equal_sign;
	
	equal_sign = ft_strchr(envp, '=');
	if (equal_sign)
		*val = ft_strdup(equal_sign + 1);
	else
		*val = ft_strdup("");
	*var = ft_substr(envp, 0, ft_env_varlen(envp));
	if (!*val || !*var)
		return (NULL);
	return(envp);
}

t_env_node	*ft_init_env_node(char *var, char *val, bool isvisible)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	if (!new)
		return (ft_env_syscall_error("Init node malloc error"));
	new->variable = var;
	new->value = val;
	new->visible = isvisible;
	return (new);
}

t_list	*ft_add_to_envlist(t_list **envlist, t_env_node *new_node)
{
	t_list	*new_envlist_node;

	new_envlist_node = ft_lstnew(new_node);
	if (!(new_envlist_node))
		return (NULL);
	ft_lstadd_back(envlist, new_envlist_node);
	return (*envlist);
}

int ft_env_varlen(char *s)
{
	int a;

	a = 0;
	while (s[a] != '\0' && s[a] != '=')
		a++;
	return (a);
}
