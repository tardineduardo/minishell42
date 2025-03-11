/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:28:51 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:26:27 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	ft_cd(t_env **ms_env, char *new_path)
{
	t_env	*current;
	//char	*final_path;

	current = *ms_env;
	if (chdir(new_path) == 0)
	{
		while (current)
		{
			if (ft_strcmp(current->variable, "PWD") == 0)
			{
				ft_ms_env_update(ms_env, "OLDPWD", current->value);
				break ;
			}
			current = current->next;
		}
		//final_path = ft_cd_path_treatment(ms_env, new_path);
		ft_ms_env_update(ms_env, "PWD", new_path);
	}
	else
	{
		if (errno == 2 || errno == 20)
			ft_printf("bash: cd: %s: No such file or directory\n", new_path);
		else if (errno == 13)
			ft_printf("bash: cd: %s: Permission denied\n", new_path);
	}
}