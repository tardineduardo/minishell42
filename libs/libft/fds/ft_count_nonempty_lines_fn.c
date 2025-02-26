/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_nonempty_lines_fn.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:02:36 by eduribei          #+#    #+#             */
/*   Updated: 2024/12/14 17:44:02 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_count_nonempty_lines_fn(char *filename)
{
	int		file;
	char	*line;
	int		count_lines;

	file = open(filename, O_RDONLY);
	if (file == -1)
		return (-1);
	count_lines = 0;
	while (1)
	{
		line = get_next_line(file);
		if (!line)
			break ;
		if (ft_is_line_empty(line))
		{
			free(line);
			continue ;
		}
		count_lines += ft_count_char(line, '\n');
		free(line);
	}
	close(file);
	return (count_lines);
}
/*
I need to change my GNL so it also returns me a flag if newline = NULL,
but the loop didn't reach EOF (indicating malloc error or read error).
In these cases, countlines should also raise an issue (ex: return -1).
*/