/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:38:15 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 16:16:59 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	ft_error_handler(char *err_msg, char *world, int exit_code, t_mem **mem)
{
	if (exit_code == 126)
		ft_error_one_hundred_six(err_msg, world);
	else if (exit_code == 127)
		ft_error_one_hundred_seven(err_msg, world);
	else if (exit_code == 2)
		ft_error_two(err_msg, world);
	else if (exit_code == 1)
		ft_error_one(err_msg, world);
	ft_clear_mem_and_exit(mem);
	exit(exit_code);
}