/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ext_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:24:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/13 17:16:11 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/environs.h"
#include "../include/execution.h"
#include <sys/stat.h>

char	**ft_ms_env_arr(t_list **ms_env)
{
	t_env_node	*cur_ms_env_node;
	t_list		*current;
	char		**ms_env_cpy;
	char		*temp_key_value;
	char		*temp_key_sign;
	int			lst_size;
	int			i;

	if (!ms_env || !*ms_env)
		return (NULL);
	lst_size = ft_lstsize(*ms_env);
	ms_env_cpy = (char **)calloc(sizeof(char *), (lst_size + 1));
	i = 0;
	current = *ms_env;
	while (current)
	{
		cur_ms_env_node = current->content;
		temp_key_sign = ft_strjoin(cur_ms_env_node->variable, "=");
		temp_key_value = ft_strjoin(temp_key_sign, cur_ms_env_node->value);
		free(temp_key_sign);
		ms_env_cpy[i] = temp_key_value;
		i++;
		current = current->next;
	}
	return (ms_env_cpy);
}

void	exec_external_cmd(t_list **ms_env, t_block_node *cmd)
{
	char	**cmd_arr;
	char	**ms_env_arr;
	struct stat sb;


	cmd_arr = cmd->cmd_arr;
	cmd_arr = update_cmd_arr(ms_env, cmd_arr);
	ms_env_arr = ft_ms_env_arr(ms_env);
	if (access(cmd_arr[0], F_OK) == 0)
	{
		if (access(cmd_arr[0], R_OK) != 0)
		{
			ft_dprintf(2, "%s: Permission denied\n", cmd_arr[0]);
			exit(126);
		}
	}
	// else
	// {
	// 	ft_dprintf(2, "%s: No such file or directory12\n", cmd_arr[0]);
	// 	exit(127);
	// }
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