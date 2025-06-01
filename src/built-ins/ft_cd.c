/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:28:51 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/01 16:17:29 by luide-ca         ###   ########.fr       */
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

void	ft_update_pwd_and_oldpwd(t_list **envlist, char *value, t_mem **mem)
{
	t_list		*trav;
	t_env_node	*current;
	char		*absolute_path;

	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, "PWD") == 0)
		{
			ft_ms_env_update_cd(envlist, "OLDPWD", current->value);
			break ;
		}
		trav = trav->next;
	}
	absolute_path = get_relative_path(value, mem);
	ft_ms_env_update_cd(envlist, "PWD", absolute_path);
	free(absolute_path);
}

int	ft_cd(t_list **envlist, char **cmd_arr, t_mem **mem)
{
	int	size_cmd_arr;

	size_cmd_arr = ft_count_items(cmd_arr);
	if (size_cmd_arr > 2)
	{
		ft_dprintf(2, "%s: too many arguments\n", cmd_arr[0]);
		return (1);
	}
	if (chdir(cmd_arr[1]) == 0)
		ft_update_pwd_and_oldpwd(envlist, cmd_arr[1], mem);
	else
	{
		if (errno == 2 || errno == 20)
			ft_dprintf(2, "%s: No such file or directory", cmd_arr[0]);
		else if (errno == 13)
			ft_dprintf(2, "%s: Permission denied", cmd_arr[0]);
		return (1);
	}
	return (0);
}
