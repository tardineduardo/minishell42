/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/07 18:05:00 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"

static t_tok_node	*ft_init_tknd(char *str, t_tok_node *node, t_tok_mem **tok)
{
	node->value = ft_strdup(str);
	if (!node->value)
		return (NULL);
	node->oper = ft_get_oper(str);
	node->heredoc_path = NULL;
	ft_index_for_parsing(str, node, tok);
	ft_free_and_null((void *)&str);
	return (node);
}

static t_tok_exit	ft_append_tknde(char **rem, t_tok_mem **tok, int tklimit)
{
	t_tok_node	*toknode;
	t_dlist		*append;
	char		*new_str;
	char		*temp;

	new_str = ft_substr((*rem), 0, tklimit);
	toknode = malloc(sizeof(t_tok_node));
	if (!toknode)
		return (TOK_ERROR);
	if (!ft_init_tknd(new_str, toknode, tok))
		return (TOK_ERROR);
	append = ft_dlstnew(toknode);
	if (!append)
		return (TOK_ERROR);
	ft_dlstadd_back(&(*tok)->toklst, append);
	temp = *rem;
	*rem = ft_strdup(&(*rem)[tklimit]);
	ft_free_and_null((void *)&temp);
	ft_strtrim_overwrite(rem, "\t ");
	if (!(*rem)[0])
		return (TOK_END);
	return (TOK_CONTINUE);
}

static t_tok_exit	ft_tokenize_remain(char **remain, t_tok_mem **tok)
{
	int			token_limit;
	t_tok_exit	detach_exit;

	token_limit = ft_find_token_limit((*remain), tok);
	detach_exit = ft_append_tknde(remain, tok, token_limit);
	if (detach_exit == TOK_ERROR)
		return (TOK_ERROR);
	if (detach_exit == TOK_END)
		return (TOK_END);
	return (TOK_CONTINUE);
}

static int	ft_capture_heredocs(t_tok_mem **tok, t_mem **mem)
{
	t_dlist		*trav;
	t_tok_node	*node;

	trav = (*tok)->toklst;
	while (trav)
	{
		node = (t_tok_node *)trav->content;
		if (!ft_process_heredoc(node, tok, mem))
			return ((*tok)->errnmb);
		trav = trav->next;
	}
	return (0);
}

int	ft_tokenize(char **line, t_mem **mem)
{
	t_tok_mem	*tok;
	t_tok_exit	exit;

	tok = (*mem)->tokenize;
	tok->remain = ft_strdup(*line);
	while (1)
	{
		exit = ft_tokenize_remain(&tok->remain, &tok);
		if (exit == TOK_ERROR)
			return (1);
		if (exit == TOK_END)
			break ;
	}
	ft_free_and_null((void *)&tok->remain);
	if (ft_check_syntax(tok->toklst, &tok) != 0)
		return (tok->errnmb);
	if (ft_capture_heredocs(&tok, mem) != 0)
		return (tok->errnmb);
	if (ft_expand_wildcards(&tok->toklst, &tok) != 0)
		return (tok->errnmb);
	return (0);
}
