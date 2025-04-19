/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_org_tok.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/19 17:02:25 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

//NULL SE ERRO
t_list	*ft_parsing(t_mem **mem)
{
	t_par_mem	*par;
	t_tok_mem	*tok;
	t_list      *parsed_tokens;
	
	tok = (*mem)->tokenize;
	par = (*mem)->parsing;
	parsed_tokens = tok->toklst;
	while (parsed_tokens)
	{
		if(!ft_append_new_org_token(tokens, &par))
			return (NULL);
		tokens = tokens->next;
	}
	update_cmd_org_tok(&par->parsedlst);
	return (par->parsedlst);
}


void	*ft_append_new_org_token(t_list *tokens, t_org_tok_mem **org_tok_mem)
{
	t_org_tok	*org_tok;
	t_list		*append;
	char		*value;
	
	value = ((t_tok_node *)tokens->content)->tokstr;
	org_tok = malloc(sizeof(t_org_tok));
	if (!org_tok)
		return (NULL);
	org_tok->value = ft_strdup(value);
	org_tok->oper = update_oper(value);
	org_tok->cmd = -1;
	org_tok->cmd_node = NULL;
	append = ft_lstnew(org_tok);
	if (!append)
		return (NULL);
	ft_lstadd_back(&(*org_tok_mem)->org_toklst, append);
	return ()
}
