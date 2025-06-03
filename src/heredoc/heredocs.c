/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/03 19:43:55 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/expand.h"
#include "../../include/readline.h"

static char	*ft_message(t_mem **mem, char *delim)
{
	char	*msg;
	char	*line;

	line = ft_itoa((*mem)->readline->count);
	msg = ft_concatenate_var(5, "\nminishell: warning: here-document at line ",
			line, " delimited by end-of-file (wanted `", delim, "\')\"\n");
	free(line);
	return (msg);
}

static void	ft_exit_loop(int fd, int exitcode, char *delim, t_mem **mem)
{
	if (!delim)
	{
		ft_clear_mem_and_exit(mem);
		close(fd);
		exit(exitcode);
	}
	ft_printf(ft_message(mem, delim));
	close(fd);
	ft_clear_mem_and_exit(mem);
	exit(exitcode);
}

static void	ft_capture_loop(char *delim, char *prompt, int fd, t_mem **mem)
{
	char	*line;

	while (1)
	{
		line = readline(prompt);
		if (!line)
		{
			if (g_signal == SIGINT)
				ft_exit_loop(fd, 130, NULL, mem);
			ft_exit_loop(fd, EXIT_SUCCESS, delim, mem);
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_expand(&line, HEREDOC, mem);
		if (!line)
			ft_exit_loop(fd, EXIT_FAILURE, delim, mem);
		ft_dprintf(fd, "%s\n", line);
		free(line);
	}
	return ;
}

static void	ft_run_heredoc_child(char *filepath, char *delim, t_mem **mem)
{
	int		fd;
	char	*prompt;

	fd = open(filepath, O_WRONLY | O_APPEND);
	if (fd < 0)
	{
		exit(EXIT_FAILURE);
		ft_clear_mem_and_exit(mem);
	}	
	heredoc_signal();
	prompt = ft_concatenate("heredoc [", delim, "] > ");
	ft_capture_loop(delim, prompt, fd, mem);
	close(fd);
	free(filepath);
	free(prompt);
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
		(*mem)->heredoc->errnum = WEXITSTATUS(status);
		unlink(filename);
		free(filename);
		return (NULL);
	}
	(*mem)->heredoc->errnum = status;
	return (filename);
}
