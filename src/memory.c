/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:26:15 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/21 22:18:37 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_clear_hd_mem(t_hd_mem **hdoc);
void ft_clear_cap_mem(t_cap_mem **cap);
void ft_clear_tok_mem(t_tok_mem **tok);
void ft_clear_env_mem(t_env_mem **env);

void	ft_init_minishell_memory(t_mem **mem, char **envp)
{
	//malloc memory struct
	*mem = malloc(sizeof(t_mem));
	if (!(*mem))
		exit(1);//improve error message and code

	(*mem)->heredoc = malloc(sizeof(t_hd_mem));
	(*mem)->capture = malloc(sizeof(t_cap_mem));
	(*mem)->tokenize = malloc(sizeof(t_tok_mem));
	(*mem)->environs = malloc(sizeof(t_env_mem));
	
	//check for errors
	if (!(*mem) || !(*mem)->heredoc || !(*mem)->capture || !(*mem)->tokenize
		|| !(*mem)->environs)
		exit(1);//improve error message and code

	//set everything to NULL
	(*mem)->capture->line = NULL;
	(*mem)->capture->trim = NULL;
	(*mem)->capture->temp = NULL;
	(*mem)->heredoc->delim = NULL;
	(*mem)->heredoc->list = NULL;
	(*mem)->heredoc->fpath_cap = NULL;
	(*mem)->tokenize->toklst = NULL;
	(*mem)->tokenize->tri_operator = NULL;
	(*mem)->tokenize->dbl_operator = NULL;
	(*mem)->tokenize->sgl_operator = NULL;
	(*mem)->tokenize->last_of_list = NULL;
	(*mem)->tokenize->new = NULL;
	(*mem)->tokenize->node = NULL;
	(*mem)->tokenize->node2 = NULL;
	(*mem)->tokenize->str = NULL;
//------------------------------------------------
	(*mem)->environs->envlist = NULL;
	(*mem)->environs->new_node = NULL;
	(*mem)->environs->result = NULL;

	//init operators
	if (!ft_init_operators(&(*mem)->tokenize))
		ft_clear_mem_and_exit(mem);

	//init environs -------LUIS
	if (!ft_init_environs(&(*mem)->environs, envp))
		ft_clear_mem_and_exit(mem);		
}

//
void	ft_clear_mem_and_exit(t_mem **mem)
{
	//Each part of the program has its own clear function
	ft_clear_hd_mem(&(*mem)->heredoc);
	ft_clear_cap_mem(&(*mem)->capture);
	ft_clear_tok_mem(&(*mem)->tokenize);
	ft_clear_env_mem(&(*mem)->environs);

	rl_clear_history();
	free(*mem);
	exit(0);
}


void	ft_clear_hd_mem(t_hd_mem **hd)
{
	if ((*hd)->delim)
		ft_free_and_null((void *)&(*hd)->delim);
	if ((*hd)->list)
	{
		ft_lstclear(&(*hd)->list, ft_hd_unlink_and_free);
		ft_free_and_null((void *)&(*hd)->list);
	}
	free(*hd);
	return ;
}

void	ft_clear_cap_mem(t_cap_mem **cap)
{
	if ((*cap)->line)
		ft_free_and_null((void *)&(*cap)->line);

	if ((*cap)->trim)
		ft_free_and_null((void *)&(*cap)->trim);

	if ((*cap)->temp)
		ft_free_and_null((void *)&(*cap)->temp);

	free(*cap);
	return ;
}

void	ft_clear_tok_mem(t_tok_mem **tok)
{
	if ((*tok)->toklst)
		ft_lstclear(&(*tok)->toklst, ft_tok_free_node_in_list);
	if ((*tok)->tri_operator)
		ft_free_str_array((*tok)->tri_operator); //FREE ARRAY OF STRINGS
	if ((*tok)->dbl_operator)
		ft_free_str_array((*tok)->dbl_operator);  //FREE ARRAY OF STRINGS
	if ((*tok)->sgl_operator)
		ft_free_and_null((void *)&(*tok)->sgl_operator);
	if ((*tok)->str)
		ft_free_and_null((void *)&(*tok)->str);

	free(*tok);
	return ;
}

void	ft_clear_env_mem(t_env_mem **env)
{
	if ((*env)->envlist)
		ft_lstclear(&(*env)->envlist, ft_env_node_free);
	if ((*env)->result)
		ft_free_and_null_str_array(&(*env)->result);
	free(*env);
}
