/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:46:08 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/07 18:11:46 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_run_command(char *line, char *ms_env[])
{
	int		i;
	int		fd;
	char 	*variable_value = "MY_VAR=42";

	i = 0;
	fd = 0;
	if (ft_strncmp(line, "env", 3) == 0)
		ft_env(ms_env, fd);
	else if (ft_strcmp(line, "pwd") == 0)
		ft_pwd(ms_env, fd);
	else if (ft_strcmp(line, "exit") == 0)
		ft_exit();
	else if (ft_strncmp(line, "echo", 4) == 0)
	{
		i = 4;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_echo(line, fd);
	}
	else if (ft_strncmp(line, "cd", 2) == 0)
	{
		i = 2;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_cd(ms_env, line, fd);
	}
	else if (ft_strncmp(line, "export", 6) == 0)
	{
		i = 6;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_export(ms_env, variable_value, fd);
	}
	// else if (ft_strncmp(line, "unset", 5) == 0)
	// 	ft_unset(line, fd);
	return ("0");
}
