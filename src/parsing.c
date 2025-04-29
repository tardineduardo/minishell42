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
#include "../include/heredoc.h"
#include "../include/tokenize.h"

t_list		*ft_classify_tokens(t_mem **mem);
void		*ft_append_new_parsed_token(t_dlist *tokens, t_par_mem **par);
void		set_positions(t_dlist **org_tokens);
t_oper		update_oper2(char *value);
void		*ft_cmd_org(t_dlist **org_tok);


// RETORNA O POINTER PARA ROOT DE AST
void	*ft_parsing(t_mem **mem) // antiga ft_ast_create()
{
	t_ast_node *root;
	t_par_mem	*par;
	t_tok_mem	*tok;

	tok = (*mem)->tokenize;
	par = (*mem)->parsing;

	// CHECAR ERROS DE SINTAXE
	if (!ft_check_syntax(tok->toklst))
	{
		//print error type like bash
		return (NULL);
	}

	par->parlst = ft_create_parlst(tok->toklst);

	root = parse_expression(&par->parlst);
	
	//print_ast(root, 0);
	//no fim de tudo, liberar a lista de toklst
	return (mem); //trocar para rotrnar o pointer para árvore.
}


t_dlist	*ft_create_parlst(t_dlist **toklst)
{
	t_dlist		*trav;
	t_dlist		*parlst;
	t_par_node	*parnode;
	int			nbr_of_blocks;

	nbr_of_blocks = count_blocks(*toklst); 
	parlst = malloc (nbr_of_blocks * sizeof(parnode));
	trav = parlst;
	while (trav )
	{
		if ()
		parse_all_redirs()

	/////// CONTINUAR AQUI


	}


	


}



int	count_blocks(t_dlist **toklst)
{
	bool		inside_paren;
	t_dlist		*trav;
	t_tok_node	*node;
	int			count;

	inside_paren = false;
	trav = *toklst;
	count = 0;
	while(trav)
	{
		node = ((t_tok_node *)trav->content);
		if (node->value[0] == '(' && !inside_paren)
			inside_paren = true;
		else if (node->value[0] == ')' && inside_paren)
			inside_paren = false;
		if (node->value[0] == '|' && !inside_paren)
			count++;
		trav = trav->next;
	}
	return (count + 1);
}




/*
                                                                                                                                     
                                                                                             
                                      ,d                                       
                                      88                                             
,adPPYba,  8b       d8  8b,dPPYba,  MM88MMM  ,adPPYYba,  8b,     ,d8
I8[    ""  `8b     d8'  88P'   `"8a   88     ""     `Y8   `Y8, ,8P'
 `"Y8ba,    `8b   d8'   88       88   88     ,adPPPPP88     )888(
aa    ]8I    `8b,d8'    88       88   88,    88,    ,88   ,d8" "8b,
`"YbbdP"'      Y88'     88       88   "Y888  `"8bbdP"Y8  8P'     `Y8
               d8'                                                                                                                   
              d8'                                                                                                                    

*/

t_syntax	ft_check_syntax(t_list *parlst)
{
	if(operator_not_supported(parlst))
		return (ERROR1);
	if (redirect_without_file(parlst))
		return (ERROR1);
	if (pipe_at_invalid_position(parlst))
		return (ERROR1);
	// if (and_or_at_invalid_positions(parlst))
	// 	return (ERROR1);
	// if (empty_parentheses(parlst))
	// 	return (ERROR1);
	return(SUCCESS_P);
}

t_syntax	ft_operator_not_supported(t_list *parlst)
{
	t_list		*trav;
	t_par_node	*parnode;

	trav = parlst;
	while(trav)
	{
		parnode = (t_par_node *)trav->content;
		if (parnode->oper != WORD)
		{
			if (ft_strcmp(parnode->oper, "|") != 0 &&
				ft_strcmp(parnode->oper, ">") != 0 &&
				ft_strcmp(parnode->oper, "<") != 0 &&
				ft_strcmp(parnode->oper, ">>") != 0 &&
				ft_strcmp(parnode->oper, "<<") != 0)
				//set par->syntax error;
				//set error exit code;
				return (ERROR1);
		}
	return(SUCCESS_P);
	}
}

t_syntax	redirect_without_file(t_list *parlst)
{
	t_list		*trav;
	t_par_node	*parnode;
	t_par_node	*next;	
	
	trav = parlst;
	while(trav)
	{
		parnode = (t_par_node *)trav->content;	
		//transformat em funcao
		if (parnode->oper == IN_R || parnode->oper == OUT_R ||
			parnode->oper == HEREDOC_R || parnode->oper == APPEND_R ||
			parnode->oper == HERESTR_R || parnode->oper == ERROR_R ||
			parnode->oper == WILDCARD_R || parnode->oper == OUT_ERROR_R)
		{
			next = (t_par_node *)trav->next->content;
			if (!next || next->oper != WORD)
			{
				//set par->syntax error;
				//set error exit code;
				return (ERROR1);
			}
		}
		trav = trav->next;
	}
	return(SUCCESS_P);
}




























/*
                                                                                          
88,dPYba,,adPYba,    ,adPPYba,  88,dPYba,,adPYba,    ,adPPYba,   8b,dPPYba,  8b       d8  
88P'   "88"    "8a  a8P_____88  88P'   "88"    "8a  a8"     "8a  88P'   "Y8  `8b     d8'  
88      88      88  8PP"""""""  88      88      88  8b       d8  88           `8b   d8'   
88      88      88  "8b,   ,aa  88      88      88  "8a,   ,a8"  88            `8b,d8'    
88      88      88   `"Ybbd8"'  88      88      88   `"YbbdP"'   88              Y88'     
                                                                                 d8'      
                                                                                d8'    */


void	*ft_init_par_memory(t_mem **mem)
{
	(*mem)->parsing = malloc(sizeof(t_par_mem));
	if (!(*mem)->parsing)
		return (NULL);
	(*mem)->parsing->parlst = NULL;
	return ((*mem)->parsing);
}

void	ft_clear_par_mem(t_par_mem **par)
{
	ft_free_and_null((void *)&(*par)->parlst);
	free(*par);
	return ;
}