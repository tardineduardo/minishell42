/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/26 19:33:37 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/expand.h"
#include "../include/parsing.h"
#include "../include/environs.h"
#include "../include/readline.h"
#include "../include/execution.h"

volatile sig_atomic_t	g_signal;

int	main(int argc, char *argv[], char *envp[])
{
	t_mem		*mem;
	int			res;

	(void)argc;
	(void)argv;
	mem = NULL;
	ft_init_minishell_memory(&mem, envp);
	ft_ms_env_add_exit_code(&(*mem).environs->envlist, "?", 0);
	while (1)
	{
		signal(SIGINT, handle_signal_prompt);
		signal(SIGQUIT, SIG_IGN);				//ignore Ctrl+\ in shell
		if(!ft_readline(&mem))
		{
			ft_clean_mem_loop(&mem);
			continue ;
		}
		res = ft_parsing(&mem);
		if (res !=0)
		{
			ft_ms_env_update_exit_code(&(*mem).environs->envlist, "?", res);
			ft_clean_mem_loop(&mem);
			continue ;
		}
		parse_expression(&mem->parsing->parlst, &mem);
		res = ft_execute(&(*mem).environs->envlist, &mem->ast->root, &mem);
		if(res != 0)
		{
			ft_ms_env_update_exit_code(&(*mem).environs->envlist, "?", res);
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
	t_rdl_mem *cap;
	t_tok_mem *tok;
	t_hdc_mem *hd;
	t_par_mem *par;
	t_ast_mem *ast;


	cap = (*mem)->readline;
	tok = (*mem)->tokenize;
	hd = (*mem)->heredoc;
	par = (*mem)->parsing;
	ast = (*mem)->ast;

	ft_dlstclear(&tok->toklst, ft_del_token_node);
	ft_lstclear(&par->parlst, ft_del_par_node);
	ft_free_ast(ast->root);
	ft_free_and_null((void *)&hd->delim);
	ft_free_and_null((void *)&cap->line);
	ft_free_and_null((void *)&tok->str);
	ft_free_and_null((void *)&tok->remain);
	tok->block_count = 0;
	tok->index_count = 0;	
	return ;
}
