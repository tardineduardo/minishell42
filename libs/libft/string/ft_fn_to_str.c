/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fn_to_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:02:36 by eduribei          #+#    #+#             */
/*   Updated: 2024/12/14 17:45:32 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static char	*ft_parse_text(int file)
{
	int		size_read;
	char	*content;
	char	*temp;
	char	buffer[BUFFER_SIZE + 1];

	content = ft_calloc(1, sizeof(char));
	if (!content)
		return (NULL);
	size_read = BUFFER_SIZE;
	while (size_read > 0)
	{
		size_read = read(file, buffer, BUFFER_SIZE);
		if (size_read == -1)
			return (free(content), NULL);
		buffer[size_read] = '\0';
		temp = content;
		content = ft_strjoin(content, buffer);
		if (!content)
		{
			free(temp);
			break ;
		}
		free(temp);
	}
	return (content);
}

char	*ft_fn_to_str(char *filename)
{
	int		file;
	char	*content;

	file = open(filename, O_RDONLY);
	if (file == -1)
	{
		ft_dprintf(STDERR_FILENO, "Error opening file \"%s\": %s.\n",
			filename, strerror(errno));
		return (NULL);
	}
	content = ft_parse_text(file);
	if (!content)
	{
		ft_dprintf(STDERR_FILENO, "Error parsing file \"%s\": %s.\n",
			filename, strerror(errno));
		close(file);
		return (NULL);
	}
	close(file);
	return (content);
}
