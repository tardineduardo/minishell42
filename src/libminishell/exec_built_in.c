/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:33:28 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/27 18:40:49 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/prototype.h"

bool	is_built_in(t_cmd_node	*cur_cmd)
{
	char	**cmd_arr;

	cmd_arr = cur_cmd->cmd_arr;
	if (ft_strncmp(cmd_arr[0], "pwd", 3) == 0 || \
			ft_strncmp(cmd_arr[0], "echo", 4) == 0 || \
			ft_strncmp(cmd_arr[0], "env", 3) == 0 || \
			ft_strncmp(cmd_arr[0], "cd", 2) == 0 || \
			ft_strncmp(cmd_arr[0], "export", 5) == 0 || \
			ft_strncmp(cmd_arr[0], "unset", 5) == 0 || \
			ft_strncmp(cmd_arr[0], "exit", 4) == 0)
		return (true);
	else
		return (false);
}

void	exec_built_in(t_list **ms_env, t_cmd_node *cmd)
{
	char	**cmd_arr;

	cmd_arr = cmd->cmd_arr;
	if (!cmd_arr || !cmd_arr[0])
	{
		perror("cmd_arr built-in executor");
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(cmd_arr[0], "pwd", 3) == 0)
		ft_pwd(ms_env);
	else if (ft_strncmp(cmd_arr[0], "echo", 4) == 0)
		ft_echo(cmd_arr[0], cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "env", 3) == 0)
		ft_env(*ms_env);
	else if (ft_strncmp(cmd_arr[0], "cd", 2) == 0)
		ft_cd(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "export", 5) == 0)
		ft_export(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "unset", 5) == 0)
		ft_unset(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "exit", 4) == 0)
		ft_exit();
	else
	{
		perror("built-in executor");
		exit(EXIT_FAILURE);
	}
}
