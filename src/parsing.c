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


t_list	*ft_parsing(t_mem **mem)
{
	t_org_tok_mem	*org_tok_mem;
	t_tok_mem	*token_mem;
	t_list      *tokens;
	
	token_mem = (*mem)->tokenize;
	tokens = token_mem->toklst;
	org_tok_mem = (*mem)->org_tokenize;
	while (tokens)
	{
		ft_append_new_org_token(tokens, &org_tok_mem);
		tokens = tokens->next;
	}
	update_cmd_org_tok(&org_tok_mem->org_toklst);
	return (org_tok_mem->org_toklst);
}
