/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/30 19:15:02 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/expand.h"

//Helpers da create file.
char	*ft_hd_validate_path(char **filepath, int *hd_count_int)
{
	char		*hd_count_str;
	char		*hd_temp;

	while (1)
	{
		if (access(*filepath, F_OK) != 0 || access(*filepath, W_OK) == 0)
			break ;
		else
		{
			(*hd_count_int)++;
			hd_count_str = ft_itoa(*hd_count_int);
			if (!hd_count_str)
				return (NULL);
			hd_temp = *filepath;
			*filepath = ft_strjoin("ms_temp_heredoc_", hd_count_str);
			ft_free_and_null((void *)&hd_count_str);
			if (!(*filepath))
				return (ft_free_and_null((void *)&hd_temp));
			ft_free_and_null((void *)&hd_temp);
		}
	}
	return (*filepath);
}

int	ft_hd_init_file(char **filepath)
{
	int		hd_open_file;

	hd_open_file = open(*filepath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (hd_open_file == -1)
	{
		ft_dprintf(STDERR_FILENO, "fd error: %s [%i])\n",
			strerror(errno), errno);
		return (-1);
	}
	close(hd_open_file);
	return (0);
}
