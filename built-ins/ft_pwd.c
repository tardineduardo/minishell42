/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/02/28 14:46:24 by luide-ca         ###   ########.fr       */
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

void	ft_pwd(char *envp[], int fd)
{
	int		i;
	char	**result;

	i = 0;
	(void)fd;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PWD", 3) == 0)
		{
			result = ft_split_char(envp[i], '=');
			ft_printf("%s\n", result[1]);
			ft_free_split(result, 2);
			return ;
		}
		i++;
	}
}