/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:09:06 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 18:09:19 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	test_input_redir(char *expanded_name, t_mem **mem, bool sngl_bi)
{
	int			fd;
	struct stat	sb;

	if (access(expanded_name, F_OK) != 0)
	{
		if (sngl_bi == true)
		{
			ft_dprintf(2, "%s: No such file or directory\n", expanded_name);
			return (1);
		}
		else
			ft_error_handler("%s: No such file or directory\n",
				expanded_name, 1, mem);
	}
	stat(expanded_name, &sb);
	if (S_ISDIR(sb.st_mode))
	{
		if (sngl_bi == true)
		{
			ft_dprintf(2, "%s: Is a directory\n", expanded_name);
			return (126);
		}
		else
			ft_error_handler("%s: Is a directory\n",
				expanded_name, 126, mem);
	}
	fd = open(expanded_name, O_RDONLY);
	if (fd == -1)
	{
		if (sngl_bi == true)
		{
			ft_dprintf(2, "%s: Permission denied\n", expanded_name);
			return (1);
		}
		else
			ft_error_handler("%s: Permission denied\n",
				expanded_name, 1, mem);
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
			if (sngl_bi == true)
			{
				ft_dprintf(2, "%s: Is a directory\n", expanded_name);
				return (126);
			}
			else
				ft_error_handler("%s: Is a directory\n",
					expanded_name, 126, mem);
		}
		fd = open(expanded_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
		{
			if (sngl_bi == true)
			{
				ft_dprintf(2, "%s: Permission denied\n", expanded_name);
				return (1);
			}
			else
				ft_error_handler("%s: Permission denied\n",
					expanded_name, 1, mem);
		}
		else
			close(fd);
	}
	else
	{
		stat(expanded_name, &sb);
		if (S_ISDIR(sb.st_mode))
		{
			if (sngl_bi == true)
			{
				ft_dprintf(2, "%s: Is a directory\n", expanded_name);
				return (126);
			}
			else
				ft_error_handler("%s: Is a directory\n",
					expanded_name, 126, mem);
		}
		fd = open(expanded_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
		{
			if (sngl_bi == true)
			{
				ft_dprintf(2, "%s: Permission denied\n", expanded_name);
				return (1);
			}
			else
				ft_error_handler("%s: Permission denied\n",
					expanded_name, 1, mem);
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
