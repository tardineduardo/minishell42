/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:51:31 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 16:29:51 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_run_command(char *line, char *envp[])
{
	int	i;

	i = 0;
	if (ft_strcmp(line, "env") == 0)
	{
		while (envp[i] != NULL)
		{
			ft_printf("%s\n", envp[i]);
			i++;
		}
	}
	else if (ft_strcmp(line, "pwd") == 0)
	{
		while (envp[i] != NULL)
		{
			if (ft_strncmp(envp[i], "PWD", 3) == 0)
			{
				ft_printf("%s\n", envp[i]);
				return ("0");
			}
			i++;
		}
	}
  else if (ft_strnstr(line, "<<", ft_strlen(line)))
	{
		(*mem)->hc_delim = ft_strtrim(line, " <");
		ft_hc_capture(&(*mem)->hc_delim, &(*mem)->hc_list, mem);
	}
	else if (ft_strcmp(line, "exit") == 0)
	{
		ft_clear_mem_and_exit(mem);
		exit(0);
	}
	return (NULL);
}
