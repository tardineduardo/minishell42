/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:26:15 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 16:27:38 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_clear_hd_mem(t_hd_mem **hdoc);
void ft_clear_cap_mem(t_cap_mem **cap);




void ft_init_minishell_memory(t_mem **mem)
{
	//malloc memory struct
	*mem = malloc(sizeof(t_mem));
	(*mem)->heredoc = malloc(sizeof(t_hd_mem));
	(*mem)->capture = malloc(sizeof(t_cap_mem));

	//check for errors
	if (!(*mem) || !(*mem)->heredoc || !(*mem)->capture)
		exit(1);//improve error message and code	

	//nullify everything
	(*mem)->capture->line = NULL;
	(*mem)->heredoc->delim = NULL;
	(*mem)->heredoc->list = NULL;
	(*mem)->heredoc->fpath_cap = NULL;

	//Init evironment variables
		//TODO Luis
}

//
void ft_clear_mem_and_exit(t_mem **mem)
{
	//Each part of the program has its own clear function
	ft_clear_hd_mem(&(*mem)->heredoc);
	ft_clear_cap_mem(&(*mem)->capture);
	rl_clear_history();
	free(*mem);
	exit(0);
}

void ft_clear_hd_mem(t_hd_mem **hd)
{
	if ((*hd)->delim)
		ft_free_and_null((void *)&(*hd)->delim);

	if ((*hd)->list)
	{
		ft_lstclear(&(*hd)->list, ft_hd_unlink_and_free);
		ft_free_and_null((void *)&(*hd)->list);
	}

	if ((*hd)->fpath_cap)
		ft_free_and_null((void *)&(*hd)->fpath_cap);
	
	free(*hd);
	return ;
}


void ft_clear_cap_mem(t_cap_mem **cap)
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


