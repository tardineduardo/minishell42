/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:21:27 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/16 15:19:14 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/builtins.h"

bool	is_built_in(char **cmd_arr)
{
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

int	exec_built_in(t_list **ms_env, char	**cmd_arr)
{
	bool	flag;
	int		res;

	flag = false;
	res = -1;
	if (!cmd_arr || !cmd_arr[0])
	{
		perror("cmd_arr built-in executor");
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(cmd_arr[0], "pwd", 3) == 0)
		res = ft_pwd(ms_env);
	else if (ft_strncmp(cmd_arr[0], "echo", 4) == 0)
		res = ft_echo(cmd_arr);
	else if (ft_strncmp(cmd_arr[0], "env", 3) == 0)
		res = ft_env(*ms_env);
	else if (ft_strncmp(cmd_arr[0], "cd", 2) == 0)
		res = ft_cd(ms_env, cmd_arr);
	else if (ft_strncmp(cmd_arr[0], "export", 5) == 0)
		res = ft_export(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "unset", 5) == 0)
		res = ft_unset(ms_env, cmd_arr[1]);
	else if (ft_strncmp(cmd_arr[0], "exit", 4) == 0)
		ft_exit(cmd_arr);
	else
	{
		perror("built-in executor");
		exit(EXIT_FAILURE);
	}
	return (res);
}