/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:46:08 by luide-ca          #+#    #+#             */
/*   Updated: 2025/02/28 18:32:37 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_run_command(char *line, char *envp[])
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	if (ft_strcmp(line, "env") == 0)
		ft_env(envp, fd);
	else if (ft_strcmp(line, "pwd") == 0)
		ft_pwd(envp, fd);
	else if (ft_strcmp(line, "exit") == 0)
		ft_exit();
	else if (ft_strncmp(line, "echo", 4) == 0)
		ft_echo(line, fd);
	else if (ft_strncmp(line, "cd", 2) == 0)
		ft_cd(line, fd);
	return ("0");
}
