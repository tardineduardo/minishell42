/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:28:51 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/07 13:13:44 by luide-ca         ###   ########.fr       */
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

void	ft_update_pwd_and_oldpwd(t_list **envlist, char *value)
{
	t_list		*trav;
	t_env_node	*current;

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
	ft_ms_env_update_cd(envlist, "PWD", value);
}

int	ft_cd(t_list **envlist, char **cmd_arr)
{
	if (cmd_arr[2] != NULL)
	{
		ft_dprintf(2, " too many arguments");
		return (1);
	}
	if (chdir(cmd_arr[1]) == 0)
		ft_update_pwd_and_oldpwd(envlist, cmd_arr[1]);
	else
	{
		if (errno == 2 || errno == 20)
			ft_dprintf(2, " No such file or directory");
		else if (errno == 13)
			ft_dprintf(2, " Permission denied");
		return (1);
	}
	return (0);
}
