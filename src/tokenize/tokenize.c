/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
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

int	ft_tokenize(char **line, t_mem **mem)
{
	t_tok_mem	*tok;
	t_tok_exit	exit;

	tok = (*mem)->tokenize;
	tok->remain = ft_strdup(*line);
	while (1)
	{
		exit = ft_tokenize_remain(&tok->remain, &tok, mem);
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
	return (0);
}

t_tok_exit	ft_tokenize_remain(char **remain, t_tok_mem **tok, t_mem **mem)
{
	int			token_limit;
	t_tok_exit	detach_exit;

	token_limit = ft_find_token_limit((*remain), tok);
	detach_exit = ft_append_tknde(remain, tok, token_limit, mem);
	(*tok)->index_count += token_limit;
	if (detach_exit == TOK_ERROR)
		return (TOK_ERROR);
	if (detach_exit == TOK_END)
		return (TOK_END);
	return (TOK_CONTINUE);
}

t_tok_exit	ft_append_tknde(char **rem, t_tok_mem **tok, int tklimit, t_mem **m)
{
	t_tok_node	*toknode;
	t_dlist		*append;
	char		*new_str;
	char		*temp;

	new_str = ft_substr((*rem), 0, tklimit);
	toknode = malloc(sizeof(t_tok_node));
	if (!toknode)
		return (TOK_ERROR);
	if (!ft_init_tknd(new_str, toknode, tok, m))
		return (TOK_ERROR);
	ft_free_and_null((void *)&new_str);
	append = ft_dlstnew(toknode);
	if (!append)
		return (TOK_ERROR);
	ft_dlstadd_back(&(*tok)->toklst, append);
	temp = *rem;
	*rem = ft_strdup(&(*rem)[tklimit]);
	ft_free_and_null((void *)&temp);
	(*tok)->index_count += ft_count_spaces(*rem);
	ft_strtrim_overwrite(rem, "\t ");
	if (!(*rem)[0])
		return (TOK_END);
	return (TOK_CONTINUE);
}

t_tok_node	*ft_init_tknd(char *newstr, t_tok_node *node, t_tok_mem **tok,
	t_mem **mem)
{
	(void)mem;
	node->value = ft_strdup(newstr);
	if (!node->value)
		return (NULL);
	node->oper = ft_get_oper(newstr);
	node->heredoc_path = NULL;
	capture_values_for_parsing_later(newstr, node, tok);
	return (node);
}

int	ft_capture_heredocs(t_tok_mem **tok, t_mem **mem)
{
	t_dlist		*trav;
	t_tok_node	*node;

	trav = (*tok)->toklst;
	while (trav)
	{
		node = (t_tok_node *)trav->content;
		if (!process_heredoc(node, tok, mem))
			return (1);
		trav = trav->next;
	}
	return (0);
}
