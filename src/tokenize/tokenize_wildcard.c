/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 01:05:11 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

#include <sys/types.h>
#include <dirent.h>


static bool ft_is_asterisk(char *token)
{



}

static t_wccase get_token_type(char *token)
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







static bool	ft_match_mid(char *filename, char *token, int lenp)
{
	if (ft_strnstr(filename, token, lenp))
		return (true);
	return (false);
}

static bool	ft_match_sta(char *filename, char *token, int lenp)
{
	if (ft_strncmp(filename, token, lenp) == 0)
		return (true);
	return (false);
}

static bool	ft_match_end(char *filename, char *token, int lenf, int lenp)
{
	char	*substr;
	int		lensub;

	if (lenp > lenf)
		return (false);
	substr = &filename[lenf - lenp];
	lensub = ft_strlen(substr);
	if (ft_strncmp(substr, token, lensub) == 0)
		return (true);
	return (false);
}

static bool	ft_match_edg(char *filename, char *token, int lenf)
{
	char **edges;

	edges = ft_split_char(token, '*');

	if (ft_match_sta(filename, edges[0], ft_strlen(edges[0])) 
		&& ft_match_end(filename, edges[1], lenf, ft_strlen(edges[1])))
	{
		ft_free_str_array(edges);
		return (true);
	}
	ft_free_str_array(edges);
	return (false);
}

static bool	ft_is_a_wildcard_match(char *filename, char *token, t_wccase type)
{
	int	lenf;
	int lenp;
	char *pattern;

	if (!filename || !token)
		return (false);

	pattern = ft_strtrim(token, "* \t");

	lenf = ft_strlen(filename);
	lenp = ft_strlen(pattern);
	if (type == PAT_STR)
		return (ft_match_sta(filename, pattern, lenp));
	if (type == PAT_END)
		return (ft_match_end(filename, pattern, lenf, lenp));
	if (type == PAT_MID)
		return (ft_match_mid(filename, pattern, lenp));
	return (ft_match_edg(filename, token, lenf));
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

static t_dlist	*ft_get_file(char *token, t_dlist *curr, t_wccase type)
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
		if (ft_is_a_wildcard_match(item->d_name, token, type))
		{
			new = ft_new_toklst_node(item->d_name, curr);
			if (!new)
				return (closedir(folder), NULL);
			ft_dlstadd_back(&wildlst, new);
		}
		item = readdir(folder);
	}
	if (!wildlst)
	{
		str = ((t_tok_node *)curr->content)->value;
		new = ft_new_toklst_node(ft_concatenate("\'", str, "\'"), curr);
		if (!new)
			return (closedir(folder), NULL);
		ft_dlstadd_back(&wildlst, new);
	}
	return (closedir(folder), wildlst);
}

static int	ft_expand_wild(t_dlist **toklist, t_dlist *trav, t_dlist *prev, t_dlist *next)
{
	char		*token;
	t_tok_node	*tok;
	t_dlist		*wilds;
	t_wccase	type;

	tok = (t_tok_node *)trav->content;
	token = tok->value;
	type = get_token_type(token);
	wilds = ft_get_file(token, trav, type);
	if (!wilds)
		return (1);
	ft_dlstinsert_between(toklist, wilds, prev, next);
	return (0);
}


static t_wcexit ft_token_has_valid_wildcard(t_dlist *trav, t_tok_mem **tkmem)
{
	t_tok_node	*currtok;
	int			ct;
	char		*st;

	currtok = (t_tok_node *)trav->content;
	if (!currtok)
	{
		(*tkmem)->errnmb = 1;
		return (W_ERROR);
	}
	st = currtok->value;
	ct = ft_split_count(currtok->value, '*');
	if (!ft_strchr(currtok->value, '*'))
		return (W_NO_ENTRIES);
	if (ct > 2 || (ct == 2 && (st[0] == '*' || st[ft_strlen(st) - 1] == '*')))
	{
		ft_dprintf(STDERR_FILENO, "minishell: error: invalid wildcard format.");
		ft_dprintf(STDERR_FILENO, " use *pat, pat*, *pat* or pat*pat.\n");
		(*tkmem)->errnmb = 1;
		return (W_INVALID);
	}
	return (W_ENTRIES_FOUND);
}

int	ft_expand_wildcards(t_dlist **toklist, t_tok_mem **tkmem)
{
	t_dlist		*trav;
	t_tok_node	*tok;
	t_wcexit	res;

	trav = *toklist;
	while (trav)
	{
		tok = (t_tok_node *)trav->content;
		if (!tok)
			return (1);
		res = ft_token_has_valid_wildcard(trav, tkmem);
		if (res == 1)
			return (1);
		if (res == 2)
		{
			trav = trav->next;
			continue ;
		}

		if (ft_expand_wild(toklist, trav, trav->prev, trav->next) != 0)
			return (W_ERROR);
		trav = trav->next;
	}
	return (EXIT_SUCCESS);
}

