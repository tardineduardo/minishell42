/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:11:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 16:19:07 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	ft_exit(char **cmd_arr, t_mem **mem)
{
	int	exit_code;
	int	size_cmd_arr;

	(void)mem;
	size_cmd_arr = ft_count_items(cmd_arr);
	if (size_cmd_arr > 2)
		ft_error_handler("%s: too many arguments", cmd_arr[0], 1, mem);
	if (cmd_arr[1] != NULL && ft_isalpha(cmd_arr[1][0]) != 0)
		ft_error_handler("%s: numeric argument required", cmd_arr[0], 2, mem);
	if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) == 0)
		ft_error_handler("%s: numeric argument required", cmd_arr[0], 2, mem);
	else if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) != 0)
	{
		exit_code = ft_atoi(cmd_arr[1]);
		ft_clear_mem_and_exit(mem);
		exit(exit_code);
	}
	ft_clear_mem_and_exit(mem);
	exit(0);
}
