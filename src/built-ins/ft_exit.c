/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:11:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 18:03:07 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **cmd_arr, t_mem **mem)
{
	int	exit_code;
	int	size_cmd_arr;

	(void)mem;
	size_cmd_arr = ft_count_items(cmd_arr);
	if (size_cmd_arr > 2)
		ft_error_handler("%s: too many arguments\n", cmd_arr[0], 1, mem);
	if (cmd_arr[1] != NULL)
	{
		if (!ft_is_numeric(cmd_arr[1]))
			ft_error_handler("%s: numeric argument required\n",
				cmd_arr[0], 2, mem);
		exit_code = ft_atoi(cmd_arr[1]);
		ft_clear_mem_and_exit(mem);
		exit((unsigned char)exit_code);
	}
	ft_clear_mem_and_exit(mem);
	exit(0);
}
