/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:09:06 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 20:56:22 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	test_input_redir(char *expanded_name, t_mem **mem, bool sngl_bi)
{
	int			fd;

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
	if (test_directory(expanded_name, mem, sngl_bi) == 126)
		return (126);
	fd = open(expanded_name, O_RDONLY);
	if (fd == -1 && test_permission(expanded_name, mem, sngl_bi) == 1)
		return (1);
	close(fd);
	return (0);
}

int	teste_output_redir(char *expanded_name, bool create, t_mem **mem,
			bool sngl_bi)
{
	int			fd;

	if (create == false)
	{
		if (test_directory(expanded_name, mem, sngl_bi) == 126)
			return (126);
		fd = open(expanded_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1 && test_permission(expanded_name, mem, sngl_bi) == 1)
			return (1);
		else
			close(fd);
	}
	else
	{
		if (test_directory(expanded_name, mem, sngl_bi) == 126)
			return (126);
		fd = open(expanded_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1 && test_permission(expanded_name, mem, sngl_bi) == 1)
			return (1);
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
