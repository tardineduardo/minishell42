/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:36:33 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 19:05:04 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	signal_statuses(int index, int *status)
{
	int	sig;

	if (WIFSIGNALED(status[index]))
	{
		sig = WTERMSIG(status[index]);
		if (sig == SIGQUIT)
		{
			ft_printf("Quit (core dumped)\n");
			return (128 + sig);
		}
		else if (sig == SIGPIPE)
		{
			// Broken pipe is normal in pipelines, don't print error
			// Just return the appropriate exit code
			return (128 + sig);
		}
		// For other signals, you might want to print them
		// but SIGPIPE is common and expected in shell pipelines
		return (128 + sig);
	}
	return (0);
}

int	signal_statuses_subshell(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
		{
			ft_printf("Quit (core dumped)\n");
			return (128 + sig);
		}
		else if (sig == SIGPIPE)
		{
			// Don't print anything for broken pipe in subshells either
			return (128 + sig);
		}
		return (128 + sig);
	}
	return (0);
}
