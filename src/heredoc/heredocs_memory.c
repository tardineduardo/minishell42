/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/30 19:15:02 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/expand.h"

void	*ft_init_hdc_memory(t_mem **mem)
{
	(*mem)->heredoc = malloc(sizeof(t_hdc_mem));
	if (!(*mem)->heredoc)
		return (NULL);
	(*mem)->heredoc->delim = NULL;
	(*mem)->heredoc->filepath = NULL;
	(*mem)->heredoc->looptemp = NULL;
	(*mem)->heredoc->loopinput = NULL;
	(*mem)->heredoc->mode = INIT_MODE;
	return ((*mem)->heredoc);
}

void	ft_clear_hdc_mem(t_hdc_mem **hd)
{
	ft_free_and_null((void *)&(*hd)->delim);
	free(*hd);
	return ;
}
