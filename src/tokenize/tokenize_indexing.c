/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_indexing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 01:06:32 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

static bool	ft_is_operator(char *string)
{
	if (ft_strcmp("&&", string) == 0)
		return (true);
	else if (ft_strcmp("||", string) == 0)
		return (true);
	else if (ft_strcmp("(", string) == 0)
		return (true);
	else if (ft_strcmp(")", string) == 0)
		return (true);
	else if (ft_strcmp("|", string) == 0)
		return (true);
	return (false);
}

void	ft_index_for_parsing(char *str, t_tok_node *node, t_tok_mem **tok)
{
	t_tok_node	*previous;

	if (ft_dlstlast((*tok)->toklst))
		previous = (t_tok_node *)(ft_dlstlast((*tok)->toklst)->content);
	else
		previous = NULL;
	if (ft_is_operator(str))
	{
		node->block_index = -1;
		if (previous && previous->block_index != -1)
			(*tok)->block_count++;
	}
	else
		node->block_index = (*tok)->block_count;
	(*tok)->previous = node;
}
