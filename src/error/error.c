/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:29:59 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 18:03:57 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_handle_exec_error(const char *context)
{
	perror(context);
	exit(EXIT_FAILURE);
}

int	ft_error_handler(char *err_msg, char *world, int exit_code, t_mem **mem)
{
	ft_dprintf(2, err_msg, world);
	ft_clear_mem_and_exit(mem);
	exit(exit_code);
}
