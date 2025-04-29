/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:51:22 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/28 19:30:36 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_signal_prompt(int signo)
{
	(void)signo;
	ft_putchar_fd('\n', 1);
    rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
    return ;
}

void	handle_signal_cmd(int signo)
{
	(void)signo;
	signal(SIGINT, SIG_DFL);
	//clean everything that needs to be cleaned
	exit(130);
}