/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:09:06 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 17:23:52 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int test_input_redir(char *expanded_name, t_mem **mem, bool sngl_bi)
{
    int         fd;
    struct stat sb;

    if (access(expanded_name, F_OK) != 0)
    {
        return (ft_error_handler("%s: No such file or directory\n",
            expanded_name, 1, mem, sngl_bi));
    }
    stat(expanded_name, &sb);
    if (S_ISDIR(sb.st_mode))
    {
        return (ft_error_handler("%s: Is a directory\n", expanded_name, 126, mem, sngl_bi));
    }
    fd = open(expanded_name, O_RDONLY);
    if (fd == -1)
    {
        return (ft_error_handler("%s: Permission denied\n", expanded_name, 1, mem, sngl_bi));
    }
    close(fd);
    return (0);
}

int	teste_output_redir(char *expanded_name, bool create, t_mem **mem,
			bool sngl_bi)
{
	int			fd;
	struct stat	sb;


	if (create == false)
	{
		stat(expanded_name, &sb);
		if (S_ISDIR(sb.st_mode))
		{
			if(sngl_bi == true)
				return (ft_error_handler("%s: Is a directory\n", expanded_name, 126, mem, sngl_bi));
			else
				ft_error_handler("%s: Is a directory\n", expanded_name, 126, mem, sngl_bi);
		}
		fd = open(expanded_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
		{
			if (sngl_bi == true)
				return (ft_error_handler("%s: Permission denied\n", expanded_name, 1, mem, sngl_bi));
			else
				ft_error_handler("%s: Permission denied\n", expanded_name, 1, mem, sngl_bi);
		}
		else
			close(fd);
	}
	else
	{
		stat(expanded_name, &sb);
		if (S_ISDIR(sb.st_mode))
		{
			if(sngl_bi == true)
				return (ft_error_handler("%s: Is a directory\n", expanded_name, 126, mem, sngl_bi));
			else
				ft_error_handler("%s: Is a directory\n", expanded_name, 126, mem, sngl_bi);
		}
		fd = open(expanded_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			if (sngl_bi == true)
				return (ft_error_handler("%s: Permission denied\n", expanded_name, 1, mem, sngl_bi));
			else
				ft_error_handler("%s: Permission denied\n", expanded_name, 1, mem, sngl_bi);
		}
		else
			close(fd);
	}
	return (0);
}

bool	save_termios(struct termios *saved)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, saved) == 0)
			return (true);
	}
	return (false);
}

bool	restore_termios(struct termios *saved)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcsetattr(STDIN_FILENO, TCSANOW, saved) == 0)
			return (true);
	}
	return (false);
}
