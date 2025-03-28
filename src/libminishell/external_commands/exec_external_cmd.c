/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:33:54 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/28 15:32:18 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"

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

void	exec_external_cmd(t_list **ms_env, t_cmd_node *cmd)
{
	char	**cmd_arr;
	char	**ms_env_arr;

	cmd_arr = cmd->cmd_arr;
	cmd_arr = update_cmd_arr(ms_env, cmd_arr);
	ms_env_arr = ft_ms_env_arr(ms_env);
	/*
		Studying about if is necessary to pass environ when calling execve, I understood
		that if I pass the binary with the right path (or the first arg from execve) it isnt
		necessary to pass environ (or the third param from execve) TODO: align this with edu
	*/
	if (execve(cmd_arr[0], cmd_arr, ms_env_arr) == -1)
	{
		//perror(cmd_arr[0]);
		// make a error msg: command not found
		//exit("command not found");
		fprintf(stderr, "%s: command not found\n", cmd_arr[0]);
		exit(EXIT_FAILURE);
	}
}
