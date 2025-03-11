/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:20 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/11 11:46:48 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_env(t_env **ms_env, int fd)
{
	t_env	*current;

	current = *ms_env;
	while (current)
	{
		// TODO: ft_printf and ft_dprintf from edu is not working properly, in the case below it prints twice the first arg.
		ft_dprintf(fd, "%s=%s\n", current->variable, current->value);
		current = current->next;
	}
}