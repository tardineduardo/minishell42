/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:24:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/01 16:35:12 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"
#include "../../include/execution.h"
#include <sys/stat.h>

char    **ft_ms_env_arr(t_list **ms_env, t_mem **mem)
{
    t_list      *current_list_node;
    t_env_node  *cur_ms_env_node;
    char        *temp_key_value;
    char        *temp_key_sign;
    int         lst_size;
    int         i;

    if (!ms_env || !*ms_env)
        return (NULL);

    lst_size = ft_lstsize(*ms_env);
    if (!(*mem)->environs)
        return (NULL);
    (*mem)->environs->ms_env_cpy = calloc((lst_size + 1), sizeof(char *));
    if (!(*mem)->environs->ms_env_cpy) // Error handling for allocation
    {
        // Potentially free other parts of mem if this is a critical error
        // ft_clear_mem_and_exit(mem); // Not here, as this function returns
        return (NULL);
    }

    i = 0;
    current_list_node = *ms_env;
    while (current_list_node)
    {
        cur_ms_env_node = current_list_node->content;

        temp_key_sign = ft_strjoin(cur_ms_env_node->variable, "=");
        if (!temp_key_sign)
        {
            while (i > 0)
                free((*mem)->environs->ms_env_cpy[--i]);
            free((*mem)->environs->ms_env_cpy);
            (*mem)->environs->ms_env_cpy = NULL;
            return (NULL);
        }
        temp_key_value = ft_strjoin(temp_key_sign, cur_ms_env_node->value);
        free(temp_key_sign);
        if (!temp_key_value)
        {
            while (i > 0)
                free((*mem)->environs->ms_env_cpy[--i]);
            free((*mem)->environs->ms_env_cpy);
            (*mem)->environs->ms_env_cpy = NULL;
            return (NULL);
        }
        (*mem)->environs->ms_env_cpy[i] = temp_key_value;
        i++;
        current_list_node = current_list_node->next;
    }
    (*mem)->environs->ms_env_cpy[i] = NULL;
    return ((*mem)->environs->ms_env_cpy);
}

void	exec_external_cmd(t_list **ms_env, t_block_node *cmd, t_mem **mem)
{
	char		**cmd_arr;
	char		**ms_env_arr;
	struct stat	sb;

	cmd_arr = cmd->cmd_arr;
	if (ft_strchr(cmd_arr[0], '/'))
	{
		if (access(cmd_arr[0], F_OK) != 0)
		{
			ft_dprintf(2, "%s: No such file or directory\n", cmd_arr[0]);
			ft_clear_mem_and_exit(mem);
			exit(127);
		}
		if (access(cmd_arr[0], X_OK) != 0)
		{
			ft_dprintf(2, "%s: Permission denied\n", cmd_arr[0]);
			ft_clear_mem_and_exit(mem);
			exit(126);
		}
		if (stat(cmd_arr[0], &sb) != 0)
		{
			perror(cmd_arr[0]);
			ft_clear_mem_and_exit(mem);
			exit(127);
		}
		if (S_ISDIR(sb.st_mode))
		{
			ft_dprintf(2, "%s: Is a directory\n", cmd_arr[0]);
			ft_clear_mem_and_exit(mem);
			exit(126);
		}
	}
	else
	{
		cmd_arr = update_cmd_arr(ms_env, cmd_arr, mem);
		if (!cmd_arr)
		{
			ft_dprintf(2, "%s: command not found\n", cmd_arr[0]);
			ft_clear_mem_and_exit(mem);
			exit(127);
		}
	}
	ms_env_arr = ft_ms_env_arr(ms_env, mem);
	if (execve(cmd_arr[0], cmd_arr, ms_env_arr) == -1)
	{
		ft_dprintf(2, "%s: command not found\n", cmd_arr[0]);
		ft_clear_mem_and_exit(mem);
		exit(127);
	}
}
