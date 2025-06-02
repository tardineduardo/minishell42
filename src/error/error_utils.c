/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:29:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 16:33:50 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_error_one_hundred_six(char *err_msg, char *world)
{
	if (ft_strcmp(err_msg, "%s: Permission denied\n") == 0)
		ft_dprintf(2, "%s: Permission denied\n", world);
	else if (ft_strcmp(err_msg, "%s: Is a directory\n") == 0)
		ft_dprintf(2, "%s: Is a directory\n", world);
}

void	ft_error_one_hundred_seven(char *err_msg, char *world)
{
	if (ft_strcmp(err_msg, "%s: No such file or directory\n") == 0)
		ft_dprintf(2, "%s: No such file or directory\n", world);
	else if (ft_strcmp(err_msg, "%s: command not found\n") == 0)
		ft_dprintf(2, "%s: command not found\n", world);
	else if (ft_strcmp(err_msg, "%s: command not found\n") == 0)
		ft_dprintf(2, "%s: command not found\n", world);
}

void	ft_error_two(char *err_msg, char *world)
{
	if (ft_strcmp(err_msg, "%s: numeric argument required") == 0)
		ft_dprintf(2, "%s: numeric argument required", world);
}

void	ft_error_one(char *err_msg, char *world)
{
	if (ft_strcmp(err_msg, "%s: too many arguments") == 0)
		ft_dprintf(2, "%s: too many arguments", world);
}

void	ft_handle_exec_error(const char *context)
{
	perror(context);
	exit(EXIT_FAILURE);
}
