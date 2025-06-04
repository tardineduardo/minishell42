/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:28:51 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/03 15:15:25 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"  
#include "../../include/heredoc.h"
#include "../../include/expand.h"
#include "../../include/tokenize.h"
#include "../../include/environs.h"
#include "../../include/heredoc.h"
#include "../../include/builtins.h"

void	ft_ms_env_update_cd(t_list **envlist, char *variable, char *value)
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
}

char	*ft_get_env_value(t_list **envlist, char *env_variable)
{
	t_list		*trav;
	t_env_node	*current;
	char		*value;

	value = NULL;
	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, env_variable) == 0)
		{
			value = current->value;
			break ;
		}
		trav = trav->next;
	}
	return (value);
}

void	ft_update_pwd_and_oldpwd(t_list **envlist)
{
	char		*oldpwd;
	char		*newpwd;

	oldpwd = ft_get_env_value(envlist, "PWD");
	if (oldpwd)
		ft_ms_env_update_cd(envlist, "OLDPWD", oldpwd);
	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		ft_ms_env_update_cd(envlist, "PWD", newpwd);
		free(newpwd);
	}
}

void	ft_set_home_value(t_list **envlist)
{
	char		*home_value;
	char		*oldpwd;

	home_value = ft_get_env_value(envlist, "HOME");
	if (chdir(home_value) == 0)
	{
		oldpwd = ft_get_env_value(envlist, "PWD");
		if (oldpwd)
			ft_ms_env_update_cd(envlist, "OLDPWD", oldpwd);
		if (home_value)
			ft_ms_env_update_cd(envlist, "PWD", home_value);
	}
}

int	ft_cd(t_list **envlist, char **cmd_arr)
{
	int		size_cmd_arr;

	size_cmd_arr = ft_count_items(cmd_arr);
	if (size_cmd_arr > 2)
	{
		ft_dprintf(2, "%s: too many arguments\n", cmd_arr[0]);
		return (1);
	}
	if (size_cmd_arr == 1 || ft_strcmp(cmd_arr[1], "~") == 0)
		ft_set_home_value(envlist);
	else if (chdir(cmd_arr[1]) == 0)
		ft_update_pwd_and_oldpwd(envlist);
	else
	{
		if (errno == 2 || errno == 20)
			ft_dprintf(2, "%s: No such file or directory\n", cmd_arr[0]);
		else if (errno == 13)
			ft_dprintf(2, "%s: Permission denied\n", cmd_arr[0]);
		return (1);
	}
	return (0);
}
