/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/02 20:48:27 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/expand.h"

static void	ft_capture_loop(char *delim, int fd, t_mem **mem)
{
	char	*line;
	char	*prompt;

	while (1)
	{
		prompt = ft_concatenate("heredoc [", delim, "] > ");
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			if (g_signal == SIGINT)
			{
				ft_clear_mem_and_exit(mem);
				exit(130);
			}
			exit(0);
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_expand(&line, HEREDOC, mem);
		if (!line)
			exit(EXIT_FAILURE);
		ft_dprintf(fd, "%s\n", line);
		free(line);
	}
	return ;
}

static void	ft_run_heredoc_child(char *filepath, char *delim, t_mem **mem)
{
	int		fd;

	fd = open(filepath, O_WRONLY | O_APPEND);
	if (fd < 0)
		exit(EXIT_FAILURE);
	heredoc_signal();
	ft_capture_loop(delim, fd, mem);
	close(fd);
	free(filepath);
	ft_clear_mem_and_exit(mem);
	exit(EXIT_SUCCESS);
}

char	*ft_heredoc(char **delimiter, t_mem **mem)
{
	pid_t		pid;
	char		*filename;
	static int	hd_count_int;
	int			status;

	if (!ft_create_hd_filepath(&hd_count_int, &filename))
		return (NULL);	
	if (!ft_expand(&(*delimiter), DELIMITER, mem))
		return (NULL);
	pid = fork();
	if (pid < 0)
		return (NULL);
	if (pid == 0)
		ft_run_heredoc_child(filename, *delimiter, mem);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
	{
		(*mem)->heredoc->errnum = status;
		unlink(filename);
		free(filename);
		return (NULL);
	}
	(*mem)->heredoc->errnum = status;
	return (filename);
}
