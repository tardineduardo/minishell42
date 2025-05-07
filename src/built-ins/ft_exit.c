/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:11:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/07 14:07:44 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(char **cmd_arr)
{
	if (cmd_arr[2] != NULL)
	{
		ft_dprintf(2, " too many arguments");
		exit (1);
	}
	if (cmd_arr[1] != NULL && ft_isalpha(cmd_arr[1][0]) != 0)
	{
		ft_dprintf(2, " numeric argument required");
		exit (2);
	}
	if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) == 0)
	{
		ft_dprintf(2, " numeric argument required");
		exit (0);
	}
	else if (cmd_arr[1] != NULL && ft_atoi(cmd_arr[1]) != 0)
		exit(ft_atoi(cmd_arr[1]));
	exit(0);
}
