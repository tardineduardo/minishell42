/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:29:11 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/31 18:43:44 by luide-ca         ###   ########.fr       */
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
	g_signal = SIGINT;
}

void	handle_heredoc(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	close(STDIN_FILENO);
	g_signal = SIGINT;
}

void	heredoc_signal(void)
{
	signal(SIGINT, handle_heredoc);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
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
	//signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);   // Ctrl+C should kill
	signal(SIGQUIT, SIG_DFL);  // Ctrl+\ should quit with core dump
}