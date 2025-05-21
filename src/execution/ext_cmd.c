/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:24:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/20 13:41:25 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"
#include "../../include/execution.h"
#include <sys/stat.h>

char	**ft_ms_env_arr(t_list **ms_env, t_mem **mem)
{
	t_env_node	*cur_ms_env_node;
	char		*temp_key_value;
	char		*temp_key_sign;
	int			lst_size;
	int			i;

	if (!ms_env || !*ms_env)
		return (NULL);
	lst_size = ft_lstsize(*ms_env);
	(*mem)->environs->ms_env_cpy = calloc((lst_size + 1), sizeof(char *));
	i = 0;
	while (*ms_env)
	{
		cur_ms_env_node = (*ms_env)->content;
		temp_key_sign = ft_strjoin(cur_ms_env_node->variable, "=");
		temp_key_value = ft_strjoin(temp_key_sign, cur_ms_env_node->value);
		free(temp_key_sign);
		(*mem)->environs->ms_env_cpy[i] = temp_key_value;
		i++;
		*ms_env = (*ms_env)->next;
	}
	return ((*mem)->environs->ms_env_cpy);
}

void	exec_external_cmd(t_list **ms_env, t_block_node *cmd, t_mem **mem)
{
	char		**cmd_arr;
	char		**ms_env_arr;
	struct stat	sb;

	cmd_arr = cmd->cmd_arr;
	cmd_arr = update_cmd_arr(ms_env, cmd_arr);
	ms_env_arr = ft_ms_env_arr(ms_env, mem);
	if (access(cmd_arr[0], F_OK) == 0)
	{
		if (access(cmd_arr[0], R_OK) != 0)
		{
			ft_dprintf(2, "%s: Permission denied\n", cmd_arr[0]);
			exit(126);
		}
	}
	stat(cmd_arr[0], &sb);
	if (S_ISDIR(sb.st_mode))
	{
		ft_dprintf(2, "%s: Is a directory\n", cmd_arr[0]);
		exit(126);
	}
	if (execve(cmd_arr[0], cmd_arr, ms_env_arr) == -1)
	{
		ft_dprintf(2, "%s: command not found\n", cmd_arr[0]);
		exit(127);
	}
}
