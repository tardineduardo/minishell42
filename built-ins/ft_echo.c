/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:52:39 by luide-ca          #+#    #+#             */
/*   Updated: 2025/02/28 18:24:13 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_echo(char *line, int fd)
{
	int	i;

	(void)fd;
	i = 4;
	while (*line && i >= 0)
	{
		line++;
		i--;
	}
	ft_printf("Minishell > %s\n", line);
}