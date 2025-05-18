/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_indexing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/parsing.h"

void	capture_values_for_parsing_later(char *newstr, t_tok_node *node,
	t_tok_mem **tok)
{
	t_tok_node	*previous;

	if (ft_dlstlast((*tok)->toklst))
		previous = (t_tok_node *)(ft_dlstlast((*tok)->toklst)->content);
	else
		previous = NULL;
	if (ft_is_operator_parsing(newstr))
	{
		node->block_index = -1;
		if (previous && previous->block_index != -1)
			(*tok)->block_count++;
	}
	else
		node->block_index = (*tok)->block_count;
	node->index = (*tok)->index_count;
	(*tok)->previous = node;
}

bool	ft_is_operator_parsing(char *string)
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

t_oper	ft_get_oper(char *value)
{
	if (ft_strcmp("&&", value) == 0)
		return (AND_O);
	else if (ft_strcmp("||", value) == 0)
		return (OR_O);
	else if (ft_strcmp("(", value) == 0)
		return (GSTART_O);
	else if (ft_strcmp(")", value) == 0)
		return (GEND_O);
	else if (ft_strcmp("|", value) == 0)
		return (PIPE_O);
	else if (ft_strcmp("&", value) == 0)
		return (BCKG_O);
	else if (ft_strcmp("<", value) == 0)
		return (IN_R);
	else if (ft_strcmp(">", value) == 0)
		return (OUT_R);
	else if (ft_strcmp(">>", value) == 0)
		return (APPD_R);
	else if (ft_strcmp("2>", value) == 0)
		return (ERROR_R);
	else if (ft_strcmp("<<", value) == 0)
		return (HDC_R);
	else if (ft_strcmp("<<<", value) == 0)
		return (HSTR_R);
	else if (ft_strcmp("*", value) == 0)
		return (WILD_R);
	else if (ft_strcmp("&>", value) == 0)
		return (OERR_R);
	return (WORD);
}

int	ft_count_spaces(char *s)
{
	int	a;

	a = 0;
	while (ft_isspace(s[a]))
		a++;
	return (a);
}
