/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:29:59 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 16:33:57 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_error_handler(char *err_msg, char *world, int exit_code, t_mem **mem)
{
	if (exit_code == 126)
		ft_error_one_hundred_six(err_msg, world);
	else if (exit_code == 127)
		ft_error_one_hundred_seven(err_msg, world);
	else if (exit_code == 2)
		ft_error_two(err_msg, world);
	else if (exit_code == 1)
		ft_error_one(err_msg, world);
	ft_clear_mem_and_exit(mem);
	exit(exit_code);
}
