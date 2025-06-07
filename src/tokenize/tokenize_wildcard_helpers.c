/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard_helpers1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 15:44:44 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

t_wccase	get_token_type(char *token)
{
	int	count;
	int	len;

	len = ft_strlen(token);
	count = ft_split_count(token, '*');
	if (count == 1 && token[0] == '*' && token[len - 1] != '*')
		return (PAT_END);
	else if (count == 1 && token[0] != '*' && token[len - 1] == '*')
		return (PAT_STR);
	else if (count == 1 && token[0] == '*' && token[len - 1] == '*')
		return (PAT_MID);
	else if (count == 2 && token[0] != '*' && token[len - 1] != '*')
		return (PAT_EDG);
	else
		return (LIST_ALL);
}

t_dlist	*ft_new_toklst_node(char *filename, t_dlist *end)
{
	t_dlist		*new;
	t_tok_node	*node;

	node = malloc(sizeof(t_tok_node));
	if (!node)
		return (NULL);
	node->oper = WORD;
	node->block_index = ((t_tok_node *)end->content)->block_index;
	node->value = ft_strdup(filename);
	node->heredoc_path = NULL;
	if (!node->value)
	{
		free(node);
		return (NULL);
	}
	new = ft_dlstnew(node);
	if (!new)
	{
		free(node->value);
		free(node);
	}
	return (new);
}
