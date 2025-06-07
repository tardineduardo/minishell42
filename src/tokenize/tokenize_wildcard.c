/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/06 23:48:31 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

#include <sys/types.h>
#include <dirent.h>




static t_wccase get_pattern_type(char *pattern)
{
	int	count;
	int	len;

	len = ft_strlen(pattern);
	count = ft_split_count(pattern, '*');
	if (count == 1 && pattern[0] == '*' && pattern[len - 1] != '*')
		return (PAT_END);
	else if (count == 1 && pattern[0] != '*' && pattern[len - 1] == '*')
		return (PAT_STR);
	else if (count == 1 && pattern[0] == '*' && pattern[len - 1] == '*') 
		return (PAT_MID);
	else 
		return (PAT_EDG);
}







static bool	ft_match_mid(char *filename, char *pattern, int lenp)
{
	if (ft_strnstr(filename, pattern, lenp))
		return (true);
	return (false);
}

static bool	ft_match_sta(char *filename, char *pattern, int lenp)
{
	if (ft_strncmp(filename, pattern, lenp) == 0)
		return (true);
	return (false);
}

static bool	ft_match_end(char *filename, char *pattern, int lenf, int lenp)
{
	char	*substr;
	int		lensub;

	lensub = ft_strlen(substr);
	substr = &filename[lenf - lenp];
	if (lenp > lenf)
		return (false);
	if (ft_strncmp(substr, pattern, lensub) == 0)
		return (true);
	return (false);
}

static bool	ft_match_edg(char *filename, char *pattern, int lenf)
{
	char **edges;

	edges = ft_split_char(pattern, '*');

	if (ft_match_sta(filename, edges[0], ft_strlen(edges[0])) 
		&& ft_match_end(filename, edges[1], lenf, ft_strlen(edges[1])))
	{
		ft_free_str_array(edges);
		return (true);
	}
	ft_free_str_array(edges);
	return (false);
}

static bool	ft_is_a_wildcard_match(char *filename, char *pattern, t_wccase type)
{
	int	lenf;
	int lenp;

	if (!filename || !pattern)
		return (false);

	lenf = ft_strlen(filename);
	lenp = ft_strlen(pattern);
	if (type == PAT_STR)
		return (ft_match_sta(filename, pattern, lenp));
	if (type == PAT_END)
		return (ft_match_end(filename, pattern, lenf, lenp));
	if (type == PAT_MID)
		return (ft_match_mid(filename, pattern, lenp));
	return (ft_match_edg(filename, pattern, lenf));
}

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

static t_dlist	*ft_get_file(char *pattern, t_dlist *next, t_wccase type)
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
		if (ft_is_a_wildcard_match(item->d_name, pattern, type))
		{
			new = ft_new_toklst_node(item->d_name, next);
			if (!new)
				return (closedir(folder), NULL);
			ft_dlstadd_back(&wildlst, new);
			item = readdir(folder);
		}
	}
	if (!wildlst)
	{
		str = ((t_tok_node *)next->content)->value;
		new = ft_new_toklst_node(ft_concatenate("\'", str, "\'"), next);
		if (!new)
			return (closedir(folder), NULL);
		ft_dlstadd_back(&wildlst, new);
	}
	return (closedir(folder), wildlst);
}

static int	ft_expand_wild(t_dlist **toklist, t_dlist *trav, t_dlist *prev, t_dlist *next)
{
	char		*pattern;
	t_tok_node	*tok;
	t_dlist		*wilds;
	t_wccase	type;

	tok = (t_tok_node *)trav->content;
	pattern = tok->value;
	type = get_pattern_type(pattern);
	wilds = ft_get_file(pattern, next, type);
	if (!wilds)
		return (1);
	ft_dlstinsert_between(toklist, wilds, prev, next);
	return (0);
}


static int ft_token_has_valid_wildcard(t_dlist *trav, t_tok_mem **tkmem)
{
	t_tok_node	*currtok;
	char		*st;
	int			ct;

	currtok = (t_tok_node *)trav->content;
	if (!currtok)
	{
		(*tkmem)->errnmb = 1;
		return (1);
	}
	ct = ft_split_count(st, '*');
	if (!ft_strchr(st, '*'))
		return (2);
	st = currtok->value;
	if (ct > 2 || (ct == 2 && (st[0] == '*' || st[ft_strlen(st) - 1] == '*')))
	{
		ft_dprintf(STDERR_FILENO, "minishell: error: invalid wildcard format.");
		ft_dprintf(STDERR_FILENO, " use *pat, pat*, *pat* or pat*pat.\n");
		(*tkmem)->errnmb = 1;
		return (1);
	}
	return (0);
}

int			ft_expand_wildcards(t_dlist **toklist, t_tok_mem **tkmem)
{
	t_dlist		*trav;
	t_tok_node	*tok;
	int			res;

	trav = *toklist;
	while (trav)
	{
		tok = (t_tok_node *)trav->content;
		if (!tok)
			return (W_SYS_ERROR);
	
		res = ft_token_has_valid_wildcard(trav, tkmem);
		
		if (res == 1)
			return (W_SYS_ERROR);
		if (res == 2)
		{
			trav = trav->next;
			continue;
		}

		if (ft_expand_wild(toklist, trav, trav->prev, trav->next) != 0)
			return (W_SYS_ERROR);
		trav = trav->next;
	}
	return (EXIT_SUCCESS);
}




// static bool ft_print_wild_error_and_return(void)
// {
	


// }






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