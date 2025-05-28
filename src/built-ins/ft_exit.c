/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:11:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/27 21:52:15 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(char **cmd_arr, t_mem **mem)
{
	int	exit_code;
	int	size_cmd_arr;

	(void)mem;
	size_cmd_arr = ft_count_items(cmd_arr);
	if (size_cmd_arr > 2)
	{
		ft_dprintf(2, "%s: too many arguments", cmd_arr[0]);
		ft_clear_mem_and_exit(mem);
		exit (1);
	}
	if (cmd_arr[1] != NULL && ft_isalpha(cmd_arr[1][0]) != 0)
	{
		ft_dprintf(2, "%s: numeric argument required", cmd_arr[0]);
		ft_clear_mem_and_exit(mem);
		exit (2);
	}
	if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) == 0)
	{
		ft_dprintf(2, "%s: numeric argument required", cmd_arr[0]);
		ft_clear_mem_and_exit(mem);
		exit (2);
	}
	else if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) != 0)
	{
		exit_code = ft_atoi(cmd_arr[1]);
		ft_clear_mem_and_exit(mem);
		exit(exit_code);
	}
	//ft_clean_mem_loop(mem);
	ft_clear_mem_and_exit(mem);
	exit(0);
}
