/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 16:22:50 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_mem	*mem;

	(void)argc;
	(void)argv;
	(void)envp;

	mem = NULL;
	ft_init_minishell_memory(&mem);

	while (1)
	{
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
