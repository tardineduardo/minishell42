/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:24:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 20:36:12 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"
#include "../../include/execution.h"

int	ft_malloc_env_arr(t_list **ms_env, t_mem **mem)
{
	int			lst_size;

	lst_size = ft_lstsize_env(*ms_env);
	if (!(*mem)->environs)
		return (1);
	(*mem)->environs->ms_env_cpy = calloc((lst_size + 1), sizeof(char *));
	if (!(*mem)->environs->ms_env_cpy)
	{
		ft_clean_mem_loop(mem);
		return (1);
	}
	return (0);
}

int	ft_fill_env_arr(char *var, char *val, int i, t_mem **mem)
{
	char		*temp_key_value;
	char		*temp_key_sign;

	temp_key_sign = ft_strjoin(var, "=");
	if (!temp_key_sign)
	{
		while (i > 0)
			free((*mem)->environs->ms_env_cpy[--i]);
		free((*mem)->environs->ms_env_cpy);
		(*mem)->environs->ms_env_cpy = NULL;
		return (1);
	}
	temp_key_value = ft_strjoin(temp_key_sign, val);
	free(temp_key_sign);
	if (!temp_key_value)
	{
		while (i > 0)
			free((*mem)->environs->ms_env_cpy[--i]);
		free((*mem)->environs->ms_env_cpy);
		(*mem)->environs->ms_env_cpy = NULL;
		return (1);
	}
	(*mem)->environs->ms_env_cpy[i] = temp_key_value;
	return (0);
}

char	**ft_ms_env_arr(t_list **ms_env, t_mem **mem)
{
	t_list		*cur_list_node;
	t_env_node	*env_node;
	int			i;

	if (!ms_env || !*ms_env)
		return (NULL);
	if (ft_malloc_env_arr(ms_env, mem) != 0)
		return (NULL);
	i = 0;
	cur_list_node = *ms_env;
	while (cur_list_node)
	{
		env_node = cur_list_node->content;
		if (env_node->visible == true)
		{
			if (ft_fill_env_arr(env_node->variable,
					env_node->value, i, mem) != 0)
				return (NULL);
			i++;
		}
		cur_list_node = cur_list_node->next;
	}
	(*mem)->environs->ms_env_cpy[i] = NULL;
	return ((*mem)->environs->ms_env_cpy);
}

void	ft_check_cmd_arr_first_pos(char *world, t_mem **mem)
{
	struct stat	sb;

	if (access(world, F_OK) != 0)
		ft_error_handler("%s: No such file or directory\n", world, 127, mem);
	if (access(world, X_OK) != 0)
		ft_error_handler("%s: Permission denied\n", world, 126, mem);
	if (stat(world, &sb) != 0)
	{
		perror(world);
		ft_clear_mem_and_exit(mem);
		exit(127);
	}
	if (S_ISDIR(sb.st_mode))
		ft_error_handler("%s: Is a directory\n", world, 126, mem);
}

void	exec_external_cmd(t_list **ms_env, t_block_node *cmd, t_mem **mem)
{
	char		**cmd_arr;
	char		**ms_env_arr;

	cmd_arr = cmd->cmd_arr;
	if (ft_strchr(cmd_arr[0], '/'))
		ft_check_cmd_arr_first_pos(cmd_arr[0], mem);
	else
	{
		cmd_arr = update_cmd_arr(ms_env, cmd_arr, mem);
		if (!cmd_arr)
			ft_error_handler("%s: command not found\n", cmd_arr[0], 127, mem);
	}
	ms_env_arr = ft_ms_env_arr(ms_env, mem);
	if (execve(cmd_arr[0], cmd_arr, ms_env_arr) == -1)
		ft_error_handler("%s: command not found\n", cmd_arr[0], 127, mem);
}
