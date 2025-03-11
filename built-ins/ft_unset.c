/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:43:00 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/10 21:36:38 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_unset(t_env **ms_env, char *variable, int fd)
{
    (void)fd;
    ft_lstdelone_node_env(ms_env, variable);
}