/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/08 15:02:46 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"
#include "../../include/expand.h"

static t_dlist	*ft_get_file(char *token, t_dlist *curr, t_wccase type)
{
	t_wild_mem		wild;

	wild.folder = opendir(".");
	wild.wildlst = NULL;
	wild.item = readdir(wild.folder);
	while ((wild.item))
	{
		if (ft_is_a_wildcard_match((wild.item)->d_name, token, type))
		{
			wild.new = ft_new_toklst_node((wild.item)->d_name, curr);
			ft_dlstadd_back(&wild.wildlst, wild.new);
		}
		wild.item = readdir(wild.folder);
	}
	if (!wild.wildlst)
	{
		wild.str = ((t_tok_node *)curr->content)->value;
		wild.joined = ft_concatenate("'", wild.str, "'");
		wild.new = ft_new_toklst_node(wild.joined, curr);
		free(wild.joined);
		ft_dlstadd_back(&wild.wildlst, wild.new);
	}
	closedir(wild.folder);
	return (wild.wildlst);
}

static int	ft_expand_wild(t_dlist **toklist, t_dlist *trav, t_dlist *prev,
				t_dlist *next)
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
	ft_del_token_node(trav->content);
	free(trav);
	return (0);
}

static int	ft_check_conditon(t_wcexit res, t_dlist *trav, t_tok_mem **tkmem)
{
	t_tok_node	*prevnode;

	if (res == W_NO_WILD)
		return (0);
	if (res == W_ERROR)
		return (1);
	if (trav->prev)
	{
		prevnode = (t_tok_node *)trav->prev->content;
		if (res == W_SUCCESS && ft_is_redir(prevnode))
		{
			ft_dprintf(STDERR_FILENO, "minishell: error: ambiguous redirect.\n");
			(*tkmem)->errnmb = 1;
			return (1);
		}
	}
	return (0);
}

int	ft_expand_wildcards(t_dlist **toklist, t_tok_mem **tkmem)
{
	t_dlist		*trav;
	t_dlist		*next;
	t_tok_node	*tok;
	t_wcexit	res;

	trav = *toklist;
	while (trav)
	{
		tok = (t_tok_node *)trav->content;
		if (!tok)
			return (1);
		res = ft_token_has_valid_wildcard(trav, tkmem);
		if (ft_check_conditon(res, trav, tkmem) != 0)
			return (1);
		if (res == W_NO_WILD)
		{
			trav = trav->next;
			continue ;
		}
		next = trav->next;
		if (ft_expand_wild(toklist, trav, trav->prev, trav->next) == W_ERROR)
			return (1);
		trav = next;
	}
	return (0);
}
