/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:28:51 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/07 17:10:14 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_cd_path_treatment(char *ms_env[], char *path)
{
	char	*new_path;
	char	**result;
	char	**result_2;
	int		item_index;
	int		i;
	size_t	num_elems;

	if (ft_strncmp(path, "/", 1) == 0)
		new_path = path;
	else if (ft_strncmp(path, "../", 3) == 0)
	{
		item_index = ft_ms_env_item_index(ms_env, "PWD");
		result = ft_split_char(ms_env[item_index], '=');
		num_elems = ft_split_count(result[1], '/');
		result_2 = ft_split_char(result[1], '/');
		i = 2;
		while (*path && i >= 0)
		{
			path++;
			i--;
		}
		new_path = ft_strjoin(result_2[num_elems - 2], "/");
		new_path = ft_strjoin(new_path, path);
		ft_free_split(result, 2);
		ft_free_split(result_2, num_elems);
	}
	else if (ft_strncmp(path, "./", 2) == 0)
	{
		item_index = ft_ms_env_item_index(ms_env, "PWD");
		result = ft_split_char(ms_env[item_index], '=');
		new_path = ft_strjoin(result[1], "/");
		i = 2;
		while (*path && i >= 0)
		{
			path++;
			i--;
		}
		new_path = ft_strjoin(new_path, path);
		ft_free_split(result, 2);
	}
	else
	{
		item_index = ft_ms_env_item_index(ms_env, "PWD");
		result = ft_split_char(ms_env[item_index], '=');
		new_path = ft_strjoin(result[1], "/");
		new_path = ft_strjoin(new_path, path);
		ft_free_split(result, 2);
	}
	return (new_path);
}

void	ft_cd(char *ms_env[], char *new_path, int fd)
{
	int		i;
	int		item_index;
	char	**old_path;
	char	*final_path;

	i = 0;
	(void)fd;
	item_index = ft_ms_env_item_index(ms_env, "PWD");
	old_path = ft_split_char(ms_env[item_index], '=');
	final_path = ft_cd_path_treatment(ms_env, new_path);
	ft_ms_env_update(ms_env, "PWD", final_path);
	ft_ms_env_update(ms_env, "OLDPWD", old_path[1]);
	free(old_path);
}