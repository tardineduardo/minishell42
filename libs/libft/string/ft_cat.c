/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 20:47:25 by eduribei          #+#    #+#             */
/*   Updated: 2024/12/28 21:31:51 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_cat(const char *filename)
{
	int		fd;
	char	buffer[1024];
	size_t	bytes_read;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		write(2, "Error opening file\n", 18);
		return ;
	}
	while (1)
	{
		bytes_read = read(fd, buffer, sizeof(buffer));
		write(STDOUT_FILENO, buffer, bytes_read);
		if (bytes_read < sizeof(buffer))
			break ;
	}
	close(fd);
}
