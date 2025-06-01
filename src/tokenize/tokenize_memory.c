/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 00:41:53 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

void	*ft_init_tok_memory(t_mem **mem)
{
	(*mem)->tokenize = malloc(sizeof(t_tok_mem));
	if (!(*mem)->tokenize)
		return (NULL);
	(*mem)->tokenize->operators = NULL;
	(*mem)->tokenize->toklst = NULL;
	(*mem)->tokenize->previous = NULL;
	(*mem)->tokenize->remain = NULL;
	(*mem)->tokenize->get_delimiter = false;
	(*mem)->tokenize->block_count = 0;
	(*mem)->tokenize->errnmb = 0;
	(*mem)->tokenize->quote = Q_OFF;
	return ((*mem)->tokenize);
}

void	ft_clear_tok_mem(t_tok_mem **tok)
{
	ft_dlstclear(&(*tok)->toklst, ft_del_token_node);
	ft_free_str_array((*tok)->operators);
	ft_free_and_null((void *)&(*tok)->remain);
	free(*tok);
	return ;
}

void	ft_del_token_node(void *content)
{
	t_tok_node	*tok_node;

	if (!content)
		return ;
	tok_node = (t_tok_node *)content;
	if (tok_node->heredoc_path)
		ft_free_and_null((void *)&tok_node->heredoc_path);
	ft_free_and_null((void *)&tok_node->value);
	ft_free_and_null((void *)&tok_node);
}
