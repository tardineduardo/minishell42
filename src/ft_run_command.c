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

char *ft_run_command(t_mem **mem, char *envp[])
{
	int	i;

	i = 0;
	if (ft_strcmp((*mem)->capture->line, "env") == 0)
	{
		while (envp[i] != NULL)
		{
			ft_printf("%s\n", envp[i]);
			i++;
		}
	}
	else if (ft_strcmp((*mem)->capture->line, "pwd") == 0)
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
	else if (ft_strnstr((*mem)->capture->line, "<<", ft_strlen((*mem)->capture->line)))
	{
		(*mem)->heredoc->delim = ft_strtrim((*mem)->capture->line, " <");
		ft_hc_capture(&(*mem)->heredoc);
	}
	else if (ft_strcmp((*mem)->capture->line, "exit") == 0)
	{
		ft_clear_mem_and_exit(mem);
		exit(0);
	}
	return (NULL);
}
