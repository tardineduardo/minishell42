/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_treatment.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:22:44 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/16 17:56:58 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"

int	ft_count_items(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
		i++;
	return (i);
}

char	**get_path_options(t_list **ms_env)
{
	t_env_node	*cur_env;
	t_list		*cur_node;
	char		**path_options;

	cur_node = *ms_env;
	while (cur_node)
	{
		cur_env = cur_node->content;
		if (ft_strcmp(cur_env->variable, "PATH") == 0)
		{
			path_options = ft_split_char(cur_env->value, ':');
			return (path_options);
		}
		cur_node = cur_node->next;
	}
	return (NULL);
}

char	*get_right_path(char **path_options, char *cmd_binary)
{
	char	*test_path;
	int		i;
	int		num_options;

	i = 0;
	test_path = NULL;
	num_options = ft_count_items(path_options);
	while (i < num_options)
	{
		test_path = ft_concatenate(path_options[i], "/", cmd_binary);
		if (access(test_path, F_OK | X_OK) == 0)
			return (test_path);
		free(test_path);
		i++;
	}
	return (NULL);
}

bool	is_already_executable(char *cmd_arr)
{
	if (access(cmd_arr, F_OK | X_OK) == 0)
		return (true);
	return (false);
}

char	**update_cmd_arr(t_list **ms_env, char **cmd_arr)
{
	char	**path_options;
	char	*right_option;

	if (!is_already_executable(cmd_arr[0]))
	{
		path_options = get_path_options(ms_env);
		right_option = get_right_path(path_options, cmd_arr[0]);
		if (!right_option)
		{
			if (path_options != NULL)
				ft_free_split(path_options, sizeof(path_options) / sizeof(path_options[0]));
			free(right_option);
		}
		else
		{
			cmd_arr[0] = ft_strdup(right_option);
			ft_free_split(path_options, sizeof(path_options) / sizeof(path_options[0]));
			free(right_option);
		}
	}
	return (cmd_arr);
}
