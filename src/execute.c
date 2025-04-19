/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:51:31 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 16:29:51 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_execute(char *line, t_mem **mem)
{
	int		i;
	bool	flag = false;
	char	*delim;

	i = 0;
	if (ft_strncmp(line, "env", 3) == 0)
		ft_env((*mem)->environs->envlist);
	else if (ft_strcmp(line, "pwd") == 0)
		ft_pwd(&(*mem)->environs->envlist);
	else if (ft_strncmp(line, "echo", 4) == 0)
	{
		i = 4;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_echo(line, flag);
	}
	else if (ft_strncmp(line, "cd", 2) == 0)
	{
		i = 2;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_cd(&(*mem)->environs->envlist, line);
	}
	else if (ft_strncmp(line, "export", 6) == 0)
	{
		i = 6;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_export(&(*mem)->environs->envlist, line);
	}
	else if (ft_strncmp(line, "unset", 5) == 0)
	{
		i = 5;
		while (*line && i >= 0)
		{
			line++;
			i--;
		}
		ft_unset(&(*mem)->environs->envlist, line);
	}
	else if (ft_strnstr((*mem)->capture->line, "<<", ft_strlen((*mem)->capture->line)))
	{
		delim = ft_strtrim((*mem)->capture->line, " <");
		ft_heredoc(delim, mem);
	}
	else if (ft_strcmp((*mem)->capture->line, "exit") == 0)
		ft_clear_mem_and_exit(mem);

	return (NULL);
}
