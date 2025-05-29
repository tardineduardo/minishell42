/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/28 12:09:05 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/expand.h"

char	*ft_heredoc(char *delimiter, t_mem **mem)
{
	int			pip[2];
	pid_t		pid;
	char		*filename;
	static int	hd_count_int;

	(void)mem;
	if (!ft_hd_create_file(&hd_count_int, &filename))
		return (NULL);
	if (pipe(pip) < 0)
		return (NULL);
	pid = fork();
	if (pid < 0)
		return (NULL);
	if (pid == 0)
		run_heredoc_child(pip[1], filename, delimiter, mem);
	close(pip[1]);
	waitpid(pid, NULL, 0);
	close(pip[0]);
	return (filename);
}

void	run_heredoc_child(int write_fd, char *filepath, char *delimiter,
	t_mem **mem)
{
	char	*line;
	char	*prompt;
	int		fd;

	heredoc_signal();
	close(write_fd - 1);
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
		ft_dprintf(fd, "%s\n", ft_expand(&line, HEREDOC, mem));
		free(line);
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
