/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:28:51 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/30 15:17:27 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// char	*ft_cd_path_treatment(char *ms_env[], char *path)
// {
// 	char	*new_path;
// 	char	**result;
// 	char	**result_2;
// 	int		item_index;
// 	int		i;
// 	size_t	num_elems;

// 	if (ft_strncmp(path, "/", 1) == 0)
// 		new_path = path;
// 	else if (ft_strncmp(path, "../", 3) == 0)
// 	{
// 		item_index = ft_ms_env_item_index(ms_env, "PWD");
// 		result = ft_split_char(ms_env[item_index], '=');
// 		num_elems = ft_split_count(result[1], '/');
// 		result_2 = ft_split_char(result[1], '/');
// 		i = 2;
// 		while (*path && i >= 0)
// 		{
// 			path++;
// 			i--;
// 		}
// 		new_path = ft_strjoin(result_2[num_elems - 2], "/");
// 		new_path = ft_strjoin(new_path, path);
// 		ft_free_split(result, 2);
// 		ft_free_split(result_2, num_elems);
// 	}
// 	else if (ft_strncmp(path, "./", 2) == 0)
// 	{
// 		item_index = ft_ms_env_item_index(ms_env, "PWD");
// 		result = ft_split_char(ms_env[item_index], '=');
// 		new_path = ft_strjoin(result[1], "/");
// 		i = 2;
// 		while (*path && i >= 0)
// 		{
// 			path++;
// 			i--;
// 		}
// 		new_path = ft_strjoin(new_path, path);
// 		ft_free_split(result, 2);
// 	}
// 	else
// 	{
// 		item_index = ft_ms_env_item_index(ms_env, "PWD");
// 		result = ft_split_char(ms_env[item_index], '=');
// 		new_path = ft_strjoin(result[1], "/");
// 		new_path = ft_strjoin(new_path, path);
// 		ft_free_split(result, 2);
// 	}
// 	return (new_path);
// }

/*
	**Mandatory: cd only with relative or absolute path**

	run chdir and if the returned value is 0 (success), we also 
	update OLDPWD and PWD from our copy of environment variables.
	We also make a 'path treatment' to have it in the right format
	to update PWD value (TODO).

	In case of problem we print one of two custom msgs:
	1. "bash: cd: $(new_path): No such file or directory"
	2. "bash: cd: $(new_path): Permission denied"
*/

void	ft_ms_env_update_cd(t_list **envlist, char *variable, char *value);


int	ft_cd(t_list **envlist, char *new_path)
{
	t_list 	*trav;
	t_env_node	*current;

	trav = *envlist;
	if (chdir(new_path) == 0)
	{
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
		ft_ms_env_update_cd(envlist, "PWD", new_path);
	}
	else
	{
		if (errno == 2 || errno == 20)
			ft_printf("bash: cd: %s: No such file or directory\n", new_path);
		else if (errno == 13)
			ft_printf("bash: cd: %s: Permission denied\n", new_path);
	}
	return (0);
}


void	ft_ms_env_update_cd(t_list **envlist, char *variable, char *value)
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
}
