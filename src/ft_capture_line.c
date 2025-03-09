/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_capture_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_capture_line(void)
{
	char	*cap_line;
	char	*cap_trim;
	char	*cap_temp;

	cap_line = readline("Minishell > ");
	if (!cap_line) 
	{
		ft_dprintf(STDERR_FILENO, "readline error\n");
		return (NULL);
	}
	if (ft_strlen(cap_line) == 0)
	{
		ft_free_and_null((void *)&cap_line);
		return (NULL);
	}
	while(1)
	{
		cap_trim = ft_strtrim(cap_line, " \t");
		if (!cap_trim)
			return (NULL);
		if (cap_trim[ft_strlen(cap_trim) - 1] == '|')
		{
			ft_free_and_null((void *)&cap_trim);
			cap_temp = cap_line;
			cap_line = ft_strjoin(cap_line, (readline("> ")));			
			if (!cap_line)
			{
				ft_free_and_null((void *)&cap_temp);
				return (NULL);
			}
			ft_free_and_null((void *)&cap_temp);
			continue ;
		}
		ft_free_and_null((void *)&cap_trim);
		break ;
	}
	add_history(cap_line);
	return (cap_line);
}
