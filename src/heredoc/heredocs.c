/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 23:49:32 by eduribei         ###   ########.fr       */
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
		ft_printf("ft_capture_loop 1\n");
		prompt = ft_concatenate("heredoc [", delim, "] > ");
		ft_printf("ft_capture_loop 2\n");
		line = readline(prompt);
		line = NULL;
		ft_printf("ft_capture_loop 3\n");
		free(prompt);
		ft_printf("ft_capture_loop 4\n");
		if (!line)
		{
			ft_printf("ft_capture_loop 5\n");
			if (g_signal == SIGINT)
			{
				ft_printf("ft_capture_loop 6\n");
				ft_clear_mem_and_exit(mem);
				ft_printf("ft_capture_loop 7\n");
				ft_printf("EXIT_FAILURE = %i\n", EXIT_FAILURE);
				exit(EXIT_FAILURE);
			}
			ft_printf("ft_capture_loop 8\n");
		}
		ft_printf("ft_capture_loop 9\n");
		if (ft_strcmp(line, delim) == 0)
		{
			ft_printf("ft_capture_loop 10\n");
			free(line);
			ft_printf("ft_capture_loop 11\n");
			break ;
		}
		ft_printf("ft_capture_loop 12\n");
		ft_expand(&line, HEREDOC, mem);
		ft_printf("ft_capture_loop 13\n");
		if (!line)
		{
			exit(EXIT_FAILURE);
			ft_printf("ft_capture_loop 15\n");
		}
		ft_printf("ft_capture_loop 16\n");
		ft_dprintf(fd, "%s\n", line);
		ft_printf("ft_capture_loop 17\n");
		free(line);
		ft_printf("se chegou aqui nao deu segfault\n");
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
	{
		ft_run_heredoc_child(filename, *delimiter, mem);
		ft_printf("ft_heredoc (child) 1\n");
	}
	ft_printf("ft_heredoc (parent) 2\n");
	waitpid(pid, &status, 0);
	ft_printf("ft_heredoc (parent) 3\n");
	if (WIFSIGNALED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
	{
		(*mem)->heredoc->errnum = status;
		ft_printf("ft_heredoc (parent) 4\n");
		unlink(filename);
		ft_printf("ft_heredoc (parent) 5\n");
		free(filename);
		ft_printf("ft_heredoc (parent) 6\n");
		return (NULL);
	}
	ft_printf("ft_heredoc (parent) 7\n");
	(*mem)->heredoc->errnum = status;
	ft_printf("ft_heredoc (parent) 8\n");
	return (filename);
}
