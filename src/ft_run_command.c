/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:46:08 by luide-ca          #+#    #+#             */
/*   Updated: 2025/02/27 15:06:02 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_run_command(char *line, char *envp[])
{
	int	i;

	i = 0;
	if (ft_strcmp(line, "env") == 0)
	{
		while (envp[i] != NULL)
		{
			ft_printf("%s\n", envp[i]);
			i++;
		}
	}
	else if (ft_strcmp(line, "pwd") == 0)
	{
		while (envp[i] != NULL)
		{
			if (ft_strncmp(envp[i], "PWD", 3) == 0)
			{
				ft_printf("%s\n", envp[i]);
				return ("0");
			}
			i++;
		}
	}
	return ("0");
}
