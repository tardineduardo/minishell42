/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/04/01 14:24:09 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_mem	*mem;

	(void)argc;
	(void)argv;
	mem = NULL;
	ft_init_minishell_memory(&mem, envp);
	excution_prom(&mem->environs->envlist);
	while (1)
	{
		if(!ft_readline(&mem))
		{
			ft_clean_mem_loop(&mem);
			continue ;
		}
		if(!ft_execute(mem->capture->line, &mem))
		{
			ft_clean_mem_loop(&mem);
			continue ;
		}
		ft_clean_mem_loop(&mem);
	}
	ft_clear_mem_and_exit(&mem);
	return (0);
}

void ft_clean_mem_loop(t_mem **mem)
{
	t_cap_mem *cap;
	t_tok_mem *tok;
	t_hd_mem *hd;

	cap = (*mem)->capture;
	tok = (*mem)->tokenize;
	hd = (*mem)->heredoc;

	ft_lstclear(&tok->toklst, ft_del_token_node);
	ft_free_and_null((void *)&hd->delim);
	ft_free_and_null((void *)&cap->line);
	ft_free_and_null((void *)&tok->str);
	ft_free_and_null((void *)&tok->remain);
	
	return ;
}
