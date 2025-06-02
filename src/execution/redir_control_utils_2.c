/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:09:06 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 17:11:58 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"
#include "../../include/expand.h"
#include "../../include/execution.h"

void	test_input_redir(char *expanded_name, t_mem **mem)
{
	int	fd;

	if (access(expanded_name, F_OK) != 0)
		ft_error_handler("%s: No such file or directory\n",
			expanded_name, 1, mem);
	fd = open(expanded_name, O_RDONLY);
	if (fd == -1)
		exit(1);
	close(fd);
}

void	teste_output_redir(char *expanded_name, bool create, t_mem **mem)
{
	int	fd;

	if (create == false)
	{
		fd = open(expanded_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
			ft_error_handler("%s: Permission denied\n", expanded_name, 1, mem);
		close(fd);
	}
	else
	{
		fd = open(expanded_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			ft_error_handler("%s: Permission denied\n", expanded_name, 1, mem);
		close(fd);
	}
}
