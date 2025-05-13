/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:29:11 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/08 13:31:09 by luide-ca         ###   ########.fr       */
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

void	signal_before_wait(void)
{
	signal(SIGINT, SIG_IGN);    // Ignore Ctrl+C during child run
	signal(SIGQUIT, SIG_IGN);   // Ignore Ctrl+\ during child run
}

void	signal_after_wait(void)
{
	signal(SIGINT, handle_signal_prompt);
	signal(SIGQUIT, SIG_IGN);   // Keep ignoring in shell
}

void	signal_child_process(void)
{
	signal(SIGINT, SIG_DFL);   // Ctrl+C should kill
	signal(SIGQUIT, SIG_DFL);  // Ctrl+\ should quit with core dump
}