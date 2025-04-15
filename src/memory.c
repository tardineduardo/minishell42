/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:26:15 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/30 20:12:07 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_clear_hd_mem(t_hd_mem **hdoc);
void ft_clear_cap_mem(t_cap_mem **cap);
void ft_clear_tok_mem(t_tok_mem **tok);
void ft_clear_env_mem(t_env_mem **env);
void ft_clear_exp_mem(t_exp_mem **tok);

void	ft_init_minishell_memory(t_mem **mem, char **envp)
{
	//malloc main memory struct ------------------------------------------------
	*mem = malloc(sizeof(t_mem));
	if (!(*mem))
		exit(1);//improve error message and code

	//malloc sub memory structs ------------------------------------------------
	(*mem)->heredoc = malloc(sizeof(t_hd_mem));
	(*mem)->capture = malloc(sizeof(t_cap_mem));
	(*mem)->tokenize = malloc(sizeof(t_tok_mem));
	(*mem)->environs = malloc(sizeof(t_env_mem));
	(*mem)->expand = malloc(sizeof(t_exp_mem));
	
	//check for errors ---------------------------------------------------------
	if (!(*mem) || !(*mem)->heredoc || !(*mem)->capture || !(*mem)->tokenize
		|| !(*mem)->environs || !(*mem)->expand)
		exit(1);//improve error message and code

	//set everything to NULL ---------------------------------------------------
	(*mem)->capture->line = NULL;
	(*mem)->capture->trim = NULL;
	(*mem)->capture->temp = NULL;
	(*mem)->capture->append = NULL;
	(*mem)->heredoc->delim = NULL;
	(*mem)->heredoc->list = NULL;
	(*mem)->heredoc->filepath = NULL;
	(*mem)->tokenize->toklst = NULL;
	(*mem)->tokenize->operators = NULL;
	(*mem)->tokenize->last_of_list = NULL;
	(*mem)->tokenize->new = NULL;
	(*mem)->tokenize->node = NULL;
	(*mem)->tokenize->str = NULL;
	(*mem)->tokenize->remain = NULL;
	(*mem)->environs->envlist = NULL;
	(*mem)->environs->new_node = NULL;
	(*mem)->environs->result = NULL;
	(*mem)->expand->a = 0;
	(*mem)->expand->b = 0;
	(*mem)->expand->hd_mode = EXPAND;
	(*mem)->expand->raw = NULL;
	(*mem)->expand->new = NULL;
	(*mem)->expand->value = NULL;
	(*mem)->expand->error = false;
	(*mem)->expand->sortedvars = NULL;

	//init operators -----------------------------------------------------------
	if (!ft_init_operators(&(*mem)->tokenize))
		ft_clear_mem_and_exit(mem);

	//init environs ------------------------------------------------------------
	if (!ft_init_environs(&(*mem)->environs, envp))
		ft_clear_mem_and_exit(mem);		
}

// FUNCÃO PRINCIPAL DE ENCERRAMENTO DO MINISHELL -------------------------------
void	ft_clear_mem_and_exit(t_mem **mem)
{
	//Chama cada limpador individual -------------------------------------------
	ft_clear_hd_mem(&(*mem)->heredoc);
	ft_clear_cap_mem(&(*mem)->capture);
	ft_clear_tok_mem(&(*mem)->tokenize);
	ft_clear_env_mem(&(*mem)->environs);
	ft_clear_exp_mem(&(*mem)->expand);


	rl_clear_history();
	free(*mem);
	exit(0);
}


//FUNCOES DE LIMPAR MEMEORIA DE CADA SECAO -------------------------------------

void	ft_clear_hd_mem(t_hd_mem **hd)
{
	ft_free_and_null((void *)&(*hd)->delim);
	ft_lstclear(&(*hd)->list, ft_del_heredoc_node);
	ft_free_and_null((void *)&(*hd)->list);
	free(*hd);
	return ;
}

void	ft_clear_cap_mem(t_cap_mem **cap)
{
	ft_free_and_null((void *)&(*cap)->line);
	ft_free_and_null((void *)&(*cap)->trim);
	ft_free_and_null((void *)&(*cap)->temp);
	free(*cap);
	return ;
}

void	ft_clear_tok_mem(t_tok_mem **tok)
{
	ft_lstclear(&(*tok)->toklst, ft_del_token_node);
	ft_free_str_array((*tok)->operators);
	ft_free_and_null((void *)&(*tok)->str);
	ft_free_and_null((void *)&(*tok)->remain);
	free(*tok);
	return ;
}

void	ft_clear_env_mem(t_env_mem **env)
{
	ft_lstclear(&(*env)->envlist, ft_del_env_node);
	ft_free_and_null_str_array(&(*env)->result);
	free(*env);
	return ;
}


void	ft_clear_exp_mem(t_exp_mem **exp)
{
	ft_free_and_null((void *)&(*exp)->new);
	free(*exp);
	return ;
}
