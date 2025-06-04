/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/03 20:23:39 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"
#include "../../include/builtins.h"

static int	ft_env_varlen(char *s)
{
	int	a;

	a = 0;
	while (s[a] != '\0' && s[a] != '=')
		a++;
	return (a);
}

static int	ft_set_shell(t_list **envlist)
{
	char	*dir;
	char	*shell;

	dir = ft_get_env_value(envlist, "PWD");
	if (!dir)
		return (1);
	shell = ft_concatenate("SHELL=", dir, "/minshell");
	if (!shell)
		return (1);
	ft_export(envlist, shell);
	free(shell);
	return (0);
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

void	*ft_get_var_and_value(char *envp, char **var, char **val)
{
	char	*equal_sign;

	equal_sign = ft_strchr(envp, '=');
	if (equal_sign)
		*val = ft_strdup(equal_sign + 1);
	else
		*val = ft_strdup("");
	*var = ft_substr(envp, 0, ft_env_varlen(envp));
	if (!*val || !*var)
		return (NULL);
	return (envp);
}

void	*ft_init_environs(t_env_mem **env, char **envp)
{
	char	*var;
	char	*val;

	var = NULL;
	val = NULL;
	if (!envp)
		return (NULL);
	if (!(*envp))
		return (envp);
	while (*envp != NULL)
	{
		if (!ft_get_var_and_value(*envp, &var, &val))
			return (NULL);
		(*env)->new_node = ft_init_env_node(var, val, true);
		if (!(*env)->new_node)
			return (NULL);
		if (!ft_add_to_envlist(&(*env)->envlist, (*env)->new_node))
			return (NULL);
		envp++;
	}
	if (ft_set_shell(&(*env)->envlist) != 0)
		return (NULL);
	return ((*env)->envlist);
}
