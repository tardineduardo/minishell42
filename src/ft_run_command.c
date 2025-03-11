/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:46:08 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:31:06 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_run_command(char *line, t_mem **mem)
{
	int		i;
	bool	flag = false;

	i = 0;
	if (ft_strncmp(line, "env", 3) == 0)
		ft_env(&(*mem)->ms_env);
	else if (ft_strcmp(line, "pwd") == 0)
		ft_pwd(&(*mem)->ms_env);
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
		ft_echo(line, flag);
	}
	else if (ft_strncmp(line, "cd", 2) == 0)
	{
		i = 2;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_cd(&(*mem)->ms_env, line);
	}
	else if (ft_strncmp(line, "export", 6) == 0)
	{
		i = 6;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_export(&(*mem)->ms_env, line);
	}
	else if (ft_strncmp(line, "unset", 5) == 0)
	{
		i = 5;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_unset(&(*mem)->ms_env, line);
	}
  	else if (ft_strnstr(line, "<<", ft_strlen(line)))
	{
		(*mem)->hc_delim = ft_strtrim(line, " <");
		ft_hc_capture(&(*mem)->hc_delim, &(*mem)->hc_list, mem);
	}
	else if (ft_strcmp(line, "exit") == 0)
	{
		ft_clear_mem_and_exit(mem);
		exit(0);
	}
	return (NULL);
}
