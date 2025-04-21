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

void		*ft_parsing(t_mem **mem);
t_list		*ft_classify_tokens(t_mem **mem);
void		*ft_append_new_parsed_token(t_list *tokens, t_par_mem **par);
void		set_positions(t_list **org_tokens);
t_oper		update_oper2(char *value);
void		*ft_cmd_org(t_list **org_tok);


// O QUE AST RETORNA, pARA QUEM?
void	*ft_parsing(t_mem **mem)
{
	t_ast_node *root;
	(void)root;
	(void)mem;


	// if (!ft_classify_tokens(mem))
	// 	return (NULL);
	
	// if (!ft_cmd_org(&(*mem)->org_tokenize->org_toklst))
	// 	return (NULL);

	// //DEBUG!	
	// //ft_debug_list_org(&(*mem)->org_tokenize->org_toklst);
	
	// root = parse_expression(&(*mem)->org_tokenize->org_toklst);
	// (void)root;
	// //DEBUG!
	// //print_ast(root, 0);

	return (mem); //sucesso
}






//NULL SE ERRO
// t_list	*ft_classify_tokens(t_mem **mem)
// {
// 	t_par_mem	*par;
// 	t_tok_mem	*tok;
// 	t_list      *parsed_tokens;
	
// 	tok = (*mem)->tokenize;
// 	par = (*mem)->parsing;
// 	parsed_tokens = tok->toklst;
// 	while (parsed_tokens)
// 	{
// 		if(!ft_append_new_parsed_token(parsed_tokens, &par))
// 			return (NULL);
// 		parsed_tokens = parsed_tokens->next;
// 	}
// 	set_positions(&par->parsedlst);
// 	return (par->parsedlst);
// }


void	*ft_append_new_parsed_token(t_list *tokens, t_par_mem **par)
{
	t_par_node	*par_node;
	t_list		*append;
	char		*value;
	
	value = ((t_tok_node *)tokens->content)->value;
	par_node = malloc(sizeof(t_par_node));
	if (!par_node)
		return (NULL);
	par_node->value = ft_strdup(value);
	//par_node->oper = update_oper2(value);
	par_node->position = -1;
	par_node->cmd_node = NULL;
	append = ft_lstnew(par_node);
	if (!append)
		return (NULL);
	ft_lstadd_back(&(*par)->parsedlst, append);
	return ((*par)->parsedlst);
}



// t_oper	update_oper2(char *value)
// {
// 	if (ft_strcmp("&&", value) == 0)
// 		return (O_AND);
// 	else if (ft_strcmp("||", value) == 0)
// 		return (O_OR);
// 	else if (ft_strcmp("(", value) == 0)
// 		return (O_GROUP_START);
// 	else if (ft_strcmp(")", value) == 0)
// 		return (O_GROUP_END);
// 	else if (ft_strcmp("|", value) == 0)
// 		return (O_PIPE);
// 	return (X_NONE);
// }


// void	set_positions(t_list **org_tokens)
// {
// 	int			cmd_num;
// 	bool		is_new_cmd;
// 	t_list		*curr;
// 	t_par_node	*parsnode;

// 	is_new_cmd = true;
// 	cmd_num = 0;
// 	curr = *org_tokens;
// 	while (curr)
// 	{
// 		parsnode = (t_par_node *)curr->content;
// 		if (!parsnode->oper)
// 		{
// 			if (is_new_cmd)
// 			{
// 				parsnode->position = cmd_num;
// 				is_new_cmd = false;
// 			}
// 			else
// 			parsnode->position = cmd_num;
// 		}
// 		else if (parsnode->oper == O_AND || parsnode->oper == O_OR || parsnode->oper == O_PIPE)
// 		{
// 			is_new_cmd = true;
// 			cmd_num++;
// 		}
// 		else
// 			is_new_cmd = true;
// 		curr = curr->next;
// 	}
// }