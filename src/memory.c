/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 13:26:15 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/10 14:32:26 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_init_minishell_memory(t_mem **mem, char **envp)
{
	//malloc memory struct
	*mem = malloc(sizeof(t_mem));
	if (!(*mem))
		exit(1);//improve error message and code


	//nullify everything
	(*mem)->ms_env = ft_ms_env(envp);
	(*mem)->line = NULL;
	(*mem)->hc_delim = NULL;
	(*mem)->hc_list = NULL;
	(*mem)->hc_fpath_cap = NULL;

	
	//Init evironment variables

}


void ft_clear_mem_and_exit(t_mem **mem)
{
	//Free each pointer with the necessary functions
	
	if ((*mem)->hc_delim)
		ft_free_and_null((void *)&(*mem)->hc_delim);

	if ((*mem)->hc_list)
	{
		ft_lstclear(&(*mem)->hc_list, ft_hc_unlink_and_free);
		ft_free_and_null((void *)&(*mem)->hc_list);
	}

	if ((*mem)->line)
		ft_free_and_null((void *)&(*mem)->line);

	free(*mem);

	//rl_clear_history();
	clear_history();
}







