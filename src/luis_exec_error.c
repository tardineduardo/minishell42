/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_exec_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:01:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/07 17:29:17 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exec_error(int exit_code, const char *file)
{
	if (exit_code == ERR_PERMISSION_DENIED)
		ft_dprintf(2, "minishell: %s: Permission denied\n", file);
	else if (exit_code == ERR_NO_SUCH_FILE)
		ft_dprintf(2, "minishell: %s: No such file or directory\n", file);
	exit(exit_code);
}