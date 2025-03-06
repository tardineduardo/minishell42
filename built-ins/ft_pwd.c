/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/06 15:45:21 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_free_split(char **result, int i)
{
	while (i > 0)
		free(result[--i]);
	free(result);
	return (NULL);
}

void	ft_pwd(char *ms_env[], int fd)
{
	int		i;
	char	**result;

	i = 0;
	while (ms_env[i] != NULL)
	{
		if (ft_strncmp(ms_env[i], "PWD", 3) == 0)
		{
			result = ft_split_char(ms_env[i], '=');
			ft_dprintf(fd, "%s\n", result[1]);
			ft_free_split(result, 2);
			break ;
		}
		i++;
	}
}