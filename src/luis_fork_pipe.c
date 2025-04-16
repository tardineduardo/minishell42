/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_fork_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:33:09 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/16 15:56:10 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	*ft_pipe_run(void)
{	
	int	*pipefd;

	pipefd = (int *)malloc(sizeof(int) * 2);
    if (!pipefd || pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    return (pipefd);
}

pid_t	ft_fork_control(void)
{
	pid_t	cpid;

	cpid = fork();
	if (cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (cpid);
}
