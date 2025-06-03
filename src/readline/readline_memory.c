/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:42:53 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/02 22:14:17 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/readline.h"

void	*ft_init_rdl_memory(t_mem **mem)
{
	(*mem)->readline = malloc(sizeof(t_rdl_mem));
	if (!(*mem)->readline)
		return (NULL);
	(*mem)->readline->count = 0;
	(*mem)->readline->line = NULL;
	(*mem)->readline->trim = NULL;
	(*mem)->readline->temp = NULL;
	(*mem)->readline->append = NULL;
	return ((*mem)->readline);
}

void	ft_clear_rdl_mem(t_rdl_mem **rdl)
{
	ft_free_and_null((void *)&(*rdl)->line);
	ft_free_and_null((void *)&(*rdl)->trim);
	ft_free_and_null((void *)&(*rdl)->temp);
	free(*rdl);
	return ;
}
