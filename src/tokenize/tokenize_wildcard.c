/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/06 20:06:40 by eduribei         ###   ########.fr       */
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
	char			*str;

	folder = opendir(".");
	if (!folder)
		return (NULL);
	wildlst = NULL;
	item = readdir(folder);
	while ((item))
	{
		if (ft_strnstr(item->d_name, pattern, ft_strlen(item->d_name)))
		{
			new = ft_new_toklst_node(item->d_name, end);
			if (!new)
				return (closedir(folder), NULL);
			ft_dlstadd_back(&wildlst, new);
		}
		item = readdir(folder);
	}
	if (!wildlst)
	{
		str = ((t_tok_node *)end->content)->value;
		new = ft_new_toklst_node(ft_concatenate("\'", str, "\'"), end);
		if (!new)
			return (closedir(folder), NULL);
		ft_dlstadd_back(&wildlst, new);
	}
	return (closedir(folder), wildlst);
}

static int	ft_expand_wild(t_dlist **toklist, t_dlist *start,
			t_dlist *wildnode, t_dlist *end)
{
	char		*pattern;
	t_tok_node	*tok;
	t_dlist		*wilds;

	tok = (t_tok_node *)end->content;
	pattern = tok->value;
	wilds = ft_get_file(pattern, end);
	if (!wilds)
		return (1);
	ft_dlstinsert_between(toklist, wilds, start, end->next);
	(void)wildnode;
	return (0);
}






int	ft_wildcards(t_dlist **toklist, t_tok_mem **tkmem)
{
	t_dlist		*trav;
	t_tok_node	*tok;
	int			*res;

	trav = *toklist;
	while (trav)
	{
		tok = (t_tok_node *)trav->content;
		if (!tok)
			return (1);
		res = ft_has_valid_wildcard(trav);
		if (res == 1 || res == 3)
			return (1);
		if (res == 2)
			return (0);
		
			(ft_expand_wild(toklist, start, trav, end) != 0)
				return (1);
		}
		trav = trav->next;
	}
	return (0);
}


static bool ft_print_wild_error_and_return(void)
{
	


}

int ft_has_valid_wildcard(t_dlist *trav, t_tok_mem **tkmem)
{
	t_tok_node	*currtok;
	t_tok_node	*prevtok;
	char		*st;
	int			ct;

	currtok = (t_tok_node *)trav->content;
	if (!currtok)
		return (1);
	ct = ft_split_count(st, '*');
	if (!ft_strchr('*', st))
		return (2); //2 - nowildcards
	st = currtok->value;
	if (ct > 2 || (ct == 2 && (st[0] == '*' || st[ft_strlen(st) - 1] == '*')))
	{
		ft_dprintf(STDERR_FILENO, "minishell: error: invalid wildcard format.");
		ft_dprintf(STDERR_FILENO, " use *pat, pat*, *pat* or pat*pat.\n");
	int			errnmb;
		(*tkmem)->errnum; 
		return (3);
	}
	return (0);
}




// static t_dlist	*ft_get_file(char *pattern, t_dlist *end)
// {
// 	DIR				*folder;
// 	struct dirent	*item;
// 	t_dlist			*wildlst;
// 	t_dlist			*new;
// 	char			*str;
// 	size_t			len_name;
// 	size_t			len_pat;

// 	folder = opendir(".");
// 	if (!folder)
// 		return (NULL);
// 	wildlst = NULL;
// 	item = readdir(folder);
// 	while (item)
// 	{
// 		len_name = ft_strlen(item->d_name);
// 		len_pat = ft_strlen(pattern);
// 		if (len_name >= len_pat)
// 		{
// 			if (ft_strncmp(
// 					item->d_name + (len_name - len_pat),
// 					pattern,
// 					len_pat) == 0)
// 			{
// 				new = ft_new_toklst_node(item->d_name, end);
// 				if (!new)
// 					return (closedir(folder), NULL);
// 				ft_dlstadd_back(&wildlst, new);
// 			}
// 		}
// 		item = readdir(folder);
// 	}
// 	if (!wildlst)
// 	{
// 		str = ((t_tok_node *)end->content)->value;
// 		new = ft_new_toklst_node(ft_concatenate("\'", str, "\'"), end);
// 		if (!new)
// 			return (closedir(folder), NULL);
// 		ft_dlstadd_back(&wildlst, new);
// 	}
// 	return (closedir(folder), wildlst);
// }