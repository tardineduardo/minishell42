/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_memory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/26 23:35:24 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

void	*ft_init_par_memory(t_mem **mem)
{
	(*mem)->parsing = malloc(sizeof(t_par_mem));
	if (!(*mem)->parsing)
		return (NULL);
	(*mem)->parsing->parlst = NULL;
	return ((*mem)->parsing);
}

void	ft_clear_par_mem(t_par_mem **par)
{
	ft_lstclear(&(*par)->parlst, ft_del_par_node);
	ft_free_and_null((void *)&(*par)->parlst);
	free(*par);
	return ;
}

void	ft_del_par_node(void *content)
{
	t_par_node	*par_node;

	if (!content)
		return ;
	par_node = (t_par_node *)content;
	ft_free_and_null((void *)&par_node);
}
