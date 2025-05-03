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


// RETORNA O POINTER PARA ROOT DE AST
void	*ft_parsing(t_mem **mem) // antiga ft_ast_create()
{
	t_ast_node *root;
	t_par_mem	*par;
	t_tok_mem	*tok;

	(void)root;

	tok = (*mem)->tokenize;
	par = (*mem)->parsing;

	//CHECAR ERROS DE SINTAXE
	if (!ft_check_syntax(tok->toklst))
	{

		//message for debug only ---- NEED TO GET CORRECT ERROR
		ft_printf("invalid sintax\n");
		return (NULL);
	}

	par->parlst = ft_create_parlst(&tok->toklst);

//	root = parse_expression(&par->parlst);
	
	//print_ast(root, 0);
	//no fim de tudo, liberar a lista de toklst
	return (mem); //trocar para rotrnar o pointer para árvore.
}


t_list	*ft_create_parlst(t_dlist **toklst)
{
	//t_list		*parlst;
	//t_list		*start;
	//t_par_node	*parnode;
	int			num_cmdblocks;
	int			num_parsnodes;

	
	num_cmdblocks = counter_num_cmdblocks(toklst);
	num_parsnodes = counter_num_parsnodes(toklst);

	ft_printf("num_cmdblocks = %i | num_parsnodes == %i\n\n", num_cmdblocks, num_parsnodes);

	//parlst = malloc(sizeof(t_par_node *));


	// TO DO
	// while (count < num_blocos)
	// {
	// 	start = find_block_start(toklst);
	// 	get_in();
	// 	get_out();
	// 	get_redirs();
	// 	count++;
	// }
	return (NULL);
}





int    counter_num_cmdblocks(t_dlist **toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;
	int         total_cmds;

	trav = *toklst;
	total_cmds = 0;
	while (trav)
	{
		toknode = (t_tok_node *)trav->content;
		if (toknode->block_index != -1)
			total_cmds = toknode->block_index;
		trav = trav->next;
	}
	return (total_cmds + 1);
}

int    counter_num_parsnodes(t_dlist **toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;
	int         total_parsnodes;
	int			prev_block_index;

	trav = *toklst;
	total_parsnodes = 0;
	while (trav)
	{
		if (trav == *toklst)
			prev_block_index = 0;	

		toknode = (t_tok_node *)trav->content;
		
		if (toknode->block_index != prev_block_index || prev_block_index == -1)
			total_parsnodes++; 			

		prev_block_index = toknode->block_index;	
		trav = trav->next;
	}
	return (total_parsnodes + 1);
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

t_syntax	ft_check_syntax(t_dlist *parlst)
{
	if(!operators_are_supported(parlst))
		return (ERROR1);
	if (!redirects_are_complete(parlst))
		return (ERROR1);
	// if (pipe_at_invalid_position(parlst))
	// 	return (ERROR1);
	// if (and_or_at_invalid_positions(parlst))
	// 	return (ERROR1);
	// if (empty_parentheses(parlst))
	// 	return (ERROR1);
	return(SUCCESS_P);
}

t_syntax	operators_are_supported(t_dlist *toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;

	trav = toklst;
	while(trav)
	{
		toknode = (t_tok_node *)trav->content;
		if (toknode->oper != WORD)
		{
			if (toknode->oper != PIPE_O		&&
				toknode->oper != OUT_R		&&
				toknode->oper != IN_R		&&
				toknode->oper != APPD_R	&&
				toknode->oper != HDC_R)
				//set par->syntax error;
				//set error exit code;
				return (ERROR1);
		}
		trav = trav->next;
	}
	return(SUCCESS_P);

}

t_syntax	redirects_are_complete(t_dlist *toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;
	t_tok_node	*next;	
	
	trav = toklst;
	while(trav)
	{
		toknode = (t_tok_node *)trav->content;	
		//transformat em funcao
		if (toknode->oper == IN_R		|| toknode->oper == OUT_R		||
			toknode->oper == HDC_R	|| toknode->oper == APPD_R	||
			toknode->oper == HSTR_R	|| toknode->oper == ERROR_R		||
			toknode->oper == WILD_R	|| toknode->oper == OERR_R)
		{
			if (!trav->next)
				return (ERROR1);
			next = (t_tok_node *)trav->next->content;
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