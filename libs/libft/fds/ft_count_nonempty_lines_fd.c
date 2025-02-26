/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_nonempty_lines_fd.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:02:36 by eduribei          #+#    #+#             */
/*   Updated: 2024/12/14 17:43:34 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* fd remains reusable because lseek() restores the file offset. */
int	ft_count_nonempty_lines_fd(int fd)
{
	char	*line;
	int		count_lines;
	off_t	original_position;

	original_position = lseek(fd, 0, SEEK_CUR);
	if (original_position == -1)
		return (-1);
	count_lines = 0;
	while (1)
	{
		line = get_next_line(fd);
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
	if (lseek(fd, original_position, SEEK_SET) == -1)
		return (-1);
	return (count_lines);
}
/*
I need to change my GNL so it also returns me a flag if newline = NULL,
but the loop didn't reach EOF (indicating malloc error or read error).
In these cases, countlines should also raise an issue (ex: return -1).
The lseek error handling is also strange here.
*/