/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/10 10:21:27 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	char	**ms_env;

	(void)argc;
	(void)argv;
	ms_env = ft_ms_env(envp);
	mem = NULL;
	ft_init_minishell_memory(&mem);

	while (1)
	{
		line = ft_capture_command();
		//Deixa essa comigo. Depois eu vou expandir essa função para pegar
		//heredocs e continuar a preencher o comando se a linha terminar com | 
		if (!line)
			break ;	
		add_history(line);
		ft_run_command(line, ms_env);
		//@luiscarvalhofrade você pode começar a fazer essa função, considerando
		//por exemplo if line == echo -> ft_echo(char *input, int fd). Pode
		//começar pelos builtins mais simples, tipo, echo, pwd...
		free(line);
		mem->line = ft_capture_line();
		if (!mem->line)
			continue ;
		add_history(mem->line);
		ft_run_command(mem->line, &mem);
		ft_clean_mem_loop(&mem);
	}

	ft_clear_mem_and_exit(&mem);
	return (0);
}

void ft_clean_mem_loop(t_mem **mem)
{
	if ((*mem)->hc_delim)
		ft_free_and_null((void *)&(*mem)->hc_delim);
	if ((*mem)->line)
		ft_free_and_null((void *)&(*mem)->line);
	return ;
}
