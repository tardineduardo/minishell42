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

	//CHECA ERROS DE SINTAXE
	if (!ft_check_syntax(tok->toklst))
	{
		ft_printf("invalid sintax\n");
		return (NULL);
	}

	ft_create_parlst(&tok->toklst, &par->parlst);

	print_debug_parsing(&par->parlst);

//	root = parse_expression(&par->parlst);
	
	//print_ast(root, 0);
	return (mem); //trocar para rotrnar o pointer para árvore.
}


t_list	*ft_create_parlst(t_dlist **toklst, t_list **parlst)
{
	t_list		*new;
	t_list		*lstnode;
	t_par_node	*parnode;
	int a;

	int			num_cmdblocks;
	int			num_parsnodes;


	num_cmdblocks = count_num_cmdblocks(toklst);
	num_parsnodes = count_num_parsnodes(toklst);

	ft_printf("num_cmdblocks = %i | num_parsnodes == %i\n\n", num_cmdblocks, num_parsnodes);

	// parlst2 = malloc(sizeof(t_list));
	// if (!parlst2)
	// 	return (NULL);

	a = 0;
	while(a < num_parsnodes)
	{		
		parnode = malloc(sizeof(t_par_node));
		lstnode = malloc(sizeof(t_list));
		if (!parnode || !lstnode)
			return (NULL);
		if(!init_parnode1(a, &parnode, toklst))
			return (NULL);
		new = ft_lstnew(lstnode);
		ft_lstadd_back(parlst, new);
		a++;
	}

	// a = 0;
	// while(a < num_parsnodes)
	// {		
	// 	parnode = malloc(sizeof(t_par_node));
	// 	lstnode = malloc(sizeof(t_list));
	// 	if (!parnode || !lstnode)
	// 		return (NULL);
	// 	if(!init_parnode2(a, &parnode, toklst))
	// 		return (NULL);
	// 	ft_lstadd_back(&parlst1, parnode);
	// 	a++;
	// }

	return (*parlst);
}












t_par_node *init_parnode1(int a, t_par_node **parnode, t_dlist **toklst)
{
	t_dlist		*first;
	t_tok_node	*toknode;

	first = *toklst;
	toknode = (t_tok_node *)first->content;

	(*parnode)->block_index = a;
	(*parnode)->block_node = NULL;
	(*parnode)->oper = toknode->oper;
	if(is_operator(toknode))
		return (*parnode);

	while(1)
	{
		first = *toklst;
		if (!first)
			break ;
		toknode = (t_tok_node *)first->content;
		if (toknode->block_index > a)
			break ;
		if (is_redir(toknode))
			fill_parnode_and_redir(toklst, parnode);
		else //if (is_word(toknode))
			break;
	}
	return (*parnode);
}


void *fill_parnode_and_redir(t_dlist **toklst, t_par_node **parnode)
{
	t_list			*new;
	t_tok_node		*toknode;
	t_redirs_node	*redir_node;

	//separar
	if(!(*parnode)->block_node)
	{
		(*parnode)->block_node = malloc(sizeof(t_block_node));
		(*parnode)->block_node->cmd_arr = NULL;
		(*parnode)->block_node->input_lst = NULL;
		(*parnode)->block_node->output_lst = NULL;
	}

	redir_node = malloc(sizeof(t_redirs_node));
	if(!redir_node)
		return (NULL);

	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	toknode = (t_tok_node *)(*toklst)->content;
	redir_node->type = toknode->oper;
	redir_node->create = true;

	if(toknode->oper == IN_R || toknode->oper == OUT_R || toknode->oper == APPD_R)
		redir_node->name = toknode->value; 		//AQUI PRECISA PEGAR O FULL PATH AINDA!!!!!!!!		//AQUI PRECISA PEGAR O FULL PATH AINDA!!!!!!!!		//AQUI PRECISA PEGAR O FULL PATH AINDA!!!!!!!!		//AQUI PRECISA PEGAR O FULL PATH AINDA!!!!!!!!
	if(toknode->oper == HDC_R)
		redir_node->name = toknode->heredoc_path;
	if(toknode->oper == APPD_R)
		redir_node->create = false;

	new = ft_lstnew(redir_node);
	
	ft_lstadd_back(&(*parnode)->block_node->redirs_lst, new);
	if (toknode->oper == IN_R	|| toknode->oper == HDC_R)
		ft_lstadd_back(&(*parnode)->block_node->input_lst, new);
	if (toknode->oper == OUT_R	|| toknode->oper == APPD_R)
		ft_lstadd_back(&(*parnode)->block_node->output_lst, new);

	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	return (*parnode);
}






int    count_num_cmdblocks(t_dlist **toklst)
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

int    count_num_parsnodes(t_dlist **toklst)
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
			if (toknode->oper != PIPE_O	&&
				toknode->oper != OUT_R	&&
				toknode->oper != IN_R	&&
				toknode->oper != APPD_R	&&
				toknode->oper != HDC_R)

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
		if(is_redir(toknode)) 				// se o token é um operador de redirecionamento...
		{
			if(!trav->next)
				return (ERROR1); 			// ele não pode ser último token...
			next = (t_tok_node *)trav->next->content;
			if(!next || next->oper != WORD) // e precisa ser seguido de word.
				return (ERROR1);
		}
		trav = trav->next;
	}
	return(SUCCESS_P);
}







bool is_redir(t_tok_node *toknode)
{
	if (toknode->oper == IN_R	|| toknode->oper ==  OUT_R	||  
		toknode->oper == HDC_R	|| toknode->oper ==  APPD_R)
		return (true);
	if (toknode->oper == WILD_R	|| toknode->oper ==  ERROR_R ||  
		toknode->oper == HSTR_R	|| toknode->oper ==  OERR_R)
		return (true);
	return (false);
}

bool is_pipe(t_tok_node *toknode)
{
	if (toknode->oper == PIPE_O)
		return (true);
	return (false);
}

bool is_logical(t_tok_node *toknode)
{
	if (toknode->oper == AND_O || toknode->oper == OR_O)
		return (true);
	return (false);
}

bool is_subshell(t_tok_node *toknode)
{
	if (toknode->oper == GSTART_O || toknode->oper == GEND_O)
		return (true);
	if (toknode->oper == BCKG_O)
		return (true);
	return (false);
}

bool	is_supported_token(t_tok_node *toknode)
{
	if (toknode->oper == IN_R	|| toknode->oper ==  OUT_R	||  
		toknode->oper == HDC_R	|| toknode->oper ==  APPD_R	||
		toknode->oper == PIPE_O)
		return(true);
	return (false);
}

bool	is_operator(t_tok_node *toknode)
{
	if (toknode->oper == IN_R	|| toknode->oper ==  OUT_R	||  
		toknode->oper == HDC_R	|| toknode->oper ==  APPD_R	||
		toknode->oper == PIPE_O)
		return(true);
	return (false);
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








void print_debug_parsing(t_list **parslst)
{
	int			a;
	t_list		*trav;
	t_par_node	*par;

	a = 0;
	trav = *parslst;
	while(trav)
	{
		par = (t_par_node *)trav->content;

		ft_printf("Node %i\n", a);
		ft_printf("Operator: ");
		ft_print_oper_par(par->oper);
		ft_printf("\n");
		ft_printf("Operator number: %i\n", par->oper);
		ft_printf("\n");
		ft_printf("\n");
		a++;
		trav = trav->next;
	}















}







void	ft_print_oper_par(t_oper oper)
{
	if (oper == AND_O)
		ft_printf(BRIGHT_MAGENTA "AND_O" RESET);
	else if (oper == OR_O)
		ft_printf(BRIGHT_MAGENTA "OR_O" RESET);
	else if (oper == GSTART_O)
		ft_printf(BRIGHT_MAGENTA "GSTART_O" RESET);
	else if (oper == GEND_O)
		ft_printf(BRIGHT_MAGENTA "GEND_O" RESET);
	else if (oper == PIPE_O)
		ft_printf(YELLOW "PIPE_O" RESET);
	else if (oper == BCKG_O)
		ft_printf(BRIGHT_CYAN "BCKG_O" RESET);
	else if (oper == IN_R)
		ft_printf(BRIGHT_BLUE "IN_R" RESET);
	else if (oper == OUT_R)
		ft_printf(BRIGHT_BLUE "OUT_R" RESET);
	else if (oper == APPD_R)
		ft_printf(BRIGHT_BLUE "APPD_R" RESET);
	else if (oper == ERROR_R)
		ft_printf(BRIGHT_CYAN "ERROR_R" RESET);
	else if (oper == HDC_R)
		ft_printf(BRIGHT_BLUE "HDC_R" RESET);
	else if (oper == HSTR_R)
		ft_printf(BRIGHT_CYAN "HSTR_R" RESET);
	else if (oper == WILD_R)
		ft_printf(BRIGHT_MAGENTA "WILD_R" RESET);
	else if (oper == OERR_R)
		ft_printf(BRIGHT_CYAN "OERR_R" RESET);
	else if (oper == WORD)
		ft_printf(GREEN "WORD" RESET);
	else
		ft_printf("UNKNOWN_OPERATOR (%d)", oper);
}