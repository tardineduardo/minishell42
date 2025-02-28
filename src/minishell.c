/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/02/27 13:46:36 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;

	(void)argc;
	(void)argv;
	while (1)
	{
	
		line = ft_capture_command();
		//Deixa essa comigo. Depois eu vou expandir essa função para pegar
		//heredocs e continuar a preencher o comando se a linha terminar com | 
		if (!line)
			break ;	
		add_history(line);
		ft_run_command(line, envp);
		//@luiscarvalhofrade você pode começar a fazer essa função, considerando
		//por exemplo if line == echo -> ft_echo(char *input, int fd). Pode
		//começar pelos builtins mais simples, tipo, echo, pwd...
		free(line);
	}
	rl_clear_history();
	return (0);
}
