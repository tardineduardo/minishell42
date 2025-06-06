/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/31 23:38:32 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

#include <sys/types.h>
#include <dirent.h>



static t_dlist	*ft_new_toklst_node(char *filename, t_dlist *end)
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

static t_dlist	*ft_get_file(char *pattern, t_dlist *end)
{
	DIR				*folder;
	struct dirent	*item;
	t_dlist			*wildlst;
	t_dlist			*new;

	folder = opendir(".");
	if (!folder)
		return (NULL);
	wildlst = NULL;
	while ((item))
	{
		if (ft_strnstr(item->d_name, pattern, ft_strlen(item->d_name)))
		{
			new = ft_new_toklst_node(item->d_name, end);
			if (!new)
			{
				closedir(folder);
				return (NULL);
			}
			ft_dlstadd_back(&wildlst, new);
		}
		item = readdir(folder);
	}
	closedir(folder);
	return (wildlst);
}

static int	ft_expand_wild(t_dlist **toklist, t_dlist *start,
			t_dlist *wildnode, t_dlist *end)
{
	char		*pattern;
	t_tok_node	*tok;
	t_dlist		*wilds;

	tok = (t_tok_node *)wildnode->content;
	pattern = tok->value;
	wilds = ft_get_file(pattern, end);
	if (!wilds)
		return (1);
	ft_dlst_quick_destroy_node(toklist, wildnode, ft_del_token_node);
	ft_dlstinsert_between(toklist, wilds, start, end);
	return (0);
}






int	ft_wildcards(t_dlist **toklist)
{
	t_dlist		*trav;
	t_dlist		*start;
	t_dlist		*end;
	t_tok_node	*tok;

	trav = *toklist;
	while (trav)
	{
		tok = (t_tok_node *)trav->content;
		if (tok && tok->oper == WILD_R)
		{
			start = trav->prev;
			end = trav->next;
			if (ft_expand_wild(toklist, start, trav, end) != 0)
				return (1);
		}
		trav = trav->next;
	}
	return (0);
}