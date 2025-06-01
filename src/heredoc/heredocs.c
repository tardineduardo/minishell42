/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/31 21:24:38 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/expand.h"

char	*ft_heredoc(char *delimiter, t_mem **mem)
{
	pid_t		pid;
	char		*filename;
	static int	hd_count_int;

	(void)mem;
	if (!ft_hd_create_file(&hd_count_int, &filename))
		return (NULL);
	pid = fork();
	if (pid < 0)
		return (NULL);
	if (pid == 0)
		run_heredoc_child(filename, delimiter, mem);
	waitpid(pid, NULL, 0);
	return (filename);
}

void	run_heredoc_child(char *filepath, char *delimiter,
	t_mem **mem)
{
	char	*line;
	char	*prompt;
	char	*temp;
	int		fd;

	heredoc_signal();
	fd = open(filepath, O_WRONLY | O_APPEND);
	if (fd < 0)
		exit(EXIT_FAILURE);
	while (1)
	{
		prompt = ft_concatenate("heredoc [", delimiter, "] > ");
		line = ft_capture_in_interactive_mode(prompt);
		if (!line)
			exit(EXIT_FAILURE);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		temp = ft_expand(&line, HEREDOC, mem);
		ft_dprintf(fd, "%s\n", temp);
		free(temp);
	}
	ft_free_mem_in_heredoc_child(fd, filepath, mem);
	exit(EXIT_SUCCESS);
}

char	*ft_capture_in_interactive_mode(char *prompt)
{
	char	*line;

	line = readline(prompt);
	free(prompt);
	return (line);
}

void	ft_free_mem_in_heredoc_child(int fd, char *filepath, t_mem **mem)
{
	close(fd);
	free(filepath);
	ft_clear_mem_and_exit(mem);
	return ;
}
