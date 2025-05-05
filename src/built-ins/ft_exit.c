/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:11:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/05 17:53:35 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(char **cmd_arr, t_mem **mem)
{
	int	exit_code;
	char *expanded_info;

	if (cmd_arr[2] != NULL)
	{
		ft_dprintf(2, " too many arguments");
		exit (1);
	}
	expanded_info = ft_expand(&cmd_arr[1], TOKEN, mem);
	if (cmd_arr[1] != NULL && ft_isalpha(expanded_info[0]) != 0)
	{
		ft_dprintf(2, " numeric argument required");
		exit (2);
	}
	if (cmd_arr[1] != NULL && ft_atoi(expanded_info) == 0)
	{
		ft_dprintf(2, " numeric argument required");
		exit (0);
	}
	exit_code = ft_atoi(ft_expand(&cmd_arr[1], TOKEN, mem));
	exit(exit_code);
}
