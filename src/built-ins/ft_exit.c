/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:11:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/13 19:36:30 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(char **cmd_arr, t_mem **mem)
{
	if (cmd_arr[2] != NULL)
	{
		ft_dprintf(2, "%s: too many arguments", cmd_arr[0]);
		// ft_clear_mem_and_exit(mem);
		exit (1);
	}
	if (cmd_arr[1] != NULL && ft_isalpha(cmd_arr[1][0]) != 0)
	{
		ft_dprintf(2, "%s: numeric argument required", cmd_arr[0]);
		// ft_clear_mem_and_exit(mem);
		exit (2);
	}
	if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) == 0)
	{
		ft_dprintf(2, "%s: numeric argument required", cmd_arr[0]);
		// ft_clear_mem_and_exit(mem);
		exit (0);
	}
	else if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) != 0)
	{
		exit(ft_atoi(cmd_arr[1]));
		// ft_clear_mem_and_exit(mem);
	}
	ft_clear_mem_and_exit(mem);
	exit(0);
}
