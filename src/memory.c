/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:26:15 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/03 20:16:03 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/expand.h"
#include "../include/execution.h"
#include "../include/parsing.h"
#include "../include/environs.h"
#include "../include/readline.h"

void	ft_init_minishell_memory(t_mem **mem, char **envp)
{
	*mem = malloc(sizeof(t_mem));
	if (!(*mem))
		exit(1);
	ft_init_env_memory(mem);
	ft_init_exp_memory(mem);
	ft_init_hdc_memory(mem);
	ft_init_tok_memory(mem);
	ft_init_rdl_memory(mem);
	ft_init_par_memory(mem);
	ft_init_ast_memory(mem);
	if (!(*mem)->heredoc || !(*mem)->readline || !(*mem)->tokenize
		|| !(*mem)->environs || !(*mem)->expand || !(*mem)->parsing
		|| !(*mem)->ast)
		exit(1);
	if (!ft_init_operators(&(*mem)->tokenize))
		ft_clear_mem_and_exit(mem);
	if (!ft_init_environs(&(*mem)->environs, envp))
		ft_clear_mem_and_exit(mem);
	ft_ms_env_add_exit_code(&(*mem)->environs->envlist, ft_strdup("?"), 0);
}

void	ft_clear_mem_and_exit(t_mem **mem)
{
	if ((*mem)->heredoc)
		ft_clear_hdc_mem(&(*mem)->heredoc);
	if ((*mem)->readline)
		ft_clear_rdl_mem(&(*mem)->readline);
	if ((*mem)->tokenize)
		ft_clear_tok_mem(&(*mem)->tokenize);
	if ((*mem)->expand)
		ft_clear_exp_mem(&(*mem)->expand);
	if ((*mem)->parsing)
		ft_clear_par_mem(&(*mem)->parsing);
	if ((*mem)->environs)
		ft_clear_env_mem(&(*mem)->environs);
	if ((*mem)->ast)
		ft_clear_ast_mem(&(*mem)->ast);
	rl_clear_history();
	free(*mem);
}
