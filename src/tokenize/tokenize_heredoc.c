/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 21:41:57 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"

void	*ft_process_heredoc(t_tok_node *node, t_tok_mem **tok, t_mem **mem)
{
	if ((*tok)->get_delimiter)
	{
		if (node->oper != WORD)
			return (NULL);
		node->heredoc_path = ft_heredoc(&node->value, mem);
		if (!node->heredoc_path)
		{
			(*tok)->errnmb = (*mem)->heredoc->errnum;
			return (NULL);
		}
		(*tok)->get_delimiter = false;
	}
	else
		node->heredoc_path = NULL;
	if (node->oper == HDC_R)
		(*tok)->get_delimiter = true;
	return (node);
}
