/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:20 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/06 15:45:27 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_env(char *ms_env[], char *variable_value, char *command, int fd)
{
	int	i;

	if (variable_value == NULL && command == NULL)
	{
			i = 0;
			while (ms_env[i] != NULL)
			{
				ft_dprintf(fd, "%s\n", ms_env[i]);
				i++;
			}
	}
	else if (variable_value != NULL && command == NULL)
	{
		
	}
}