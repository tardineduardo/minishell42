/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:52:39 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/06 15:42:12 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo(char **cmd_arr, t_mem **mem)
{
	int	i;
	int	j;
	bool	flag;
	(void)mem;

	i = 0;
	j = 0;
	flag = false;
	while (cmd_arr[i] != NULL)
		i++;
	j = 0;
	while (cmd_arr[j] != NULL)
	{
		if (ft_strncmp(cmd_arr[j], "-n", 2) == 0)
		{
			flag = true;
			break ;
		}
		j++;
	}
	j = 1;
	if (flag == false)
	{
		while (cmd_arr[j] != NULL)
		{
			if (j != (i - 1))
				ft_printf("%s ", cmd_arr[j]);
			else
				ft_printf("%s", cmd_arr[j]);
			j++;
		}
		ft_printf("\n");
	}
	return (0);
}
