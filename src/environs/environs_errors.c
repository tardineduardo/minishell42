/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/20 14:08:21 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"

void	*ft_env_syscall_error(char *message)
{
	ft_dprintf(STDERR_FILENO, "%s: %s [%i]\n", message, strerror(errno), errno);
	return (NULL);
}

void	*ft_env_error(char *message, t_env_mem **env)
{
	ft_dprintf(STDERR_FILENO, "Minishell: %s\n", message);
	ft_lstclear(&(*env)->envlist, ft_del_env_node);
	ft_del_env_node(&(*env)->new_node);
	ft_free_and_null_str_array(&(*env)->result);
	ft_free_and_null((void *)env);
	return (NULL);
}
