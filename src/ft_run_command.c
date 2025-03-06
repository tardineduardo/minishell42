/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:46:08 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/06 15:50:45 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_run_command(char *line, char *ms_env[])
{
	int	i;
	int	fd;
	char **result;
	size_t num_elems;

	i = 0;
	fd = 0;
	if (ft_strncmp(line, "env", 3) == 0)
	{
		num_elems = ft_split_count(line, ' ');
		if (num_elems == 1)
			ft_env(ms_env, NULL, NULL, fd);
		else if (num_elems == 2)
		{
			result = ft_split_char(line, ' ');
			ft_env(ms_env, result[1], NULL, fd);
		}
		else if (num_elems == 3)
		{
			result = ft_split_char(line, ' ');
			ft_env(ms_env, result[1], result[2], fd);
		}
	}
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
		ft_cd(line);
	}
	else if (ft_strncmp(line, "export", 6) == 0)
		ft_export(line, fd);
	// else if (ft_strncmp(line, "unset", 5) == 0)
	// 	ft_unset(line, fd);
	return ("0");
}
