/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/12 17:03:55 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/checks.h"


// RETORNA O POINTER PARA ROOT DE AST
int	ft_parsing(t_mem **mem) // antiga ft_ast_create()
{
	t_ast_node *root;
	t_par_mem	*par;
	t_tok_mem	*tok;
	int			res;

	(void)root;

	tok = (*mem)->tokenize;
	par = (*mem)->parsing;

	if (!ft_check_syntax(tok->toklst))
	{
		ft_printf("invalid sintax\n");
		return (1);
	}
	ft_create_parlst(&tok->toklst, &par->parlst, &par);
	res = 0;
	return (res); //trocar para rotrnar o pointer para árvore.
}

t_list	*ft_create_parlst(t_dlist **toklst, t_list **parlst, t_par_mem **par)
{
	int			a;
	int			num_parsnodes;

	num_parsnodes = count_num_parsnodes(toklst);
	a = 0;
	while(a < num_parsnodes)
	{		
		(*par)->parnode = malloc(sizeof(t_par_node));
		(*par)->lstnode = malloc(sizeof(t_list));
		 
		if (!(*par)->parnode || !(*par)->lstnode)
			return (NULL);
		if(!init_parnode(a, par, toklst))
			return (NULL);
		ft_lstadd_back(parlst, ft_lstnew((*par)->parnode));
		a++;
	}
	return (*parlst);
}

t_par_node *init_parnode(int a, t_par_mem **par, t_dlist **toklst)
{
	t_dlist		*first;
	t_tok_node	*toknode;

	first = *toklst;
	toknode = (t_tok_node *)first->content;

	(*par)->parnode->block_index = toknode->block_index;
	(*par)->parnode->block_node = NULL;
	if(is_pipe_logical_or_subshell(toknode))
	{
		(*par)->parnode->oper = toknode->oper;
		ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
		return ((*par)->parnode);
	}
	else 
		extract_command(a, &(*par)->parnode, toklst);
	

	(*par)->parnode->oper = CMD; 
	return ((*par)->parnode);
}

int	extract_command(int a, t_par_node **parnode, t_dlist **toklst)
{
	t_dlist		*first;
	t_tok_node	*toknode;

	while(1)
	{
		first = *toklst;
		if (!first)
			break ;
		toknode = (t_tok_node *)first->content;
		if (toknode->block_index > a || toknode->block_index == -1)
			break ;
		if (is_redir(toknode))
		{
			if(!fill_blocknode_redirs(toklst, parnode))
				return(1);
		}
		else if (is_word(toknode)) 
		{
			if(!fill_blocknode_cmdarray(toklst, parnode))
				return(1);
		}
	}
	return (0);
}

/*Essa função está muito longa e um pouco confusa pois está lidando com
input_list, output_list e redir_list. Dá para deixar menor depois.*/
void *fill_blocknode_redirs(t_dlist **toklst, t_par_node **parnode)
{
	t_list			*redirlse;
	t_list			*copy;
	t_tok_node		*toknode;
	t_redirs_node	*redirnode;
	t_oper			oper;

	if(!(*parnode)->block_node)
		intit_block_node(parnode, toklst);

	redirnode = malloc(sizeof(t_redirs_node));
	if(!redirnode)
		return (NULL);

	oper = ((t_tok_node *)(*toklst)->content)->oper;
	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	toknode = (t_tok_node *)(*toklst)->content;
	redirnode->type = oper;
	redirnode->create = true;
	if(oper == IN_R || oper == OUT_R || oper == APPD_R)
		redirnode->name = ft_strdup(toknode->value);
	if(oper == HDC_R)
		redirnode->name = ft_strdup(toknode->heredoc_path);
	if(oper == APPD_R)
		redirnode->create = false;
	redirlse = ft_lstnew(redirnode);
	ft_lstadd_back(&(*parnode)->block_node->redirs_lst, redirlse);
	copy = ft_lstnew(redirnode);
	if (oper == IN_R	|| oper == HDC_R)
		ft_lstadd_back(&(*parnode)->block_node->input_lst, copy);
	else if (oper == OUT_R	|| oper == APPD_R)
		ft_lstadd_back(&(*parnode)->block_node->output_lst, copy);
	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	return (*parnode);
}




//
// Quando dispensar inputlist e outputlist, pode essar essa de baixo.
//
// void *fill_blocknode_redirs(t_dlist **toklst, t_par_node **parnode)
// {
// 	t_list			*redirlst;
// 	t_tok_node		*toknode;
// 	t_redirs_node	*redirnode;
// 	t_oper			oper;

// 	if(!(*parnode)->block_node)
// 		intit_block_node(parnode, toklst);

// 	redirnode = malloc(sizeof(t_redirs_node));
// 	if(!redirnode)
// 		return (NULL);

// 	oper = ((t_tok_node *)(*toklst)->content)->oper;
	
// 	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);

// 	toknode = (t_tok_node *)(*toklst)->content;
// 	redirnode->type = oper;
// 	redirnode->create = true;
	
// 	if(oper == IN_R || oper == OUT_R || oper == APPD_R)
// 		redirnode->name = ft_strdup(toknode->value); 		//AQUI PRECISA PEGAR O FULL PATH
// 	if(oper == HDC_R)
// 		redirnode->name = ft_strdup(toknode->heredoc_path); //O PATH DO HEREDOC JA VEM PRONTO
// 	if(oper == APPD_R)
// 		redirnode->create = false;
// 	redirlst = ft_lstnew(redirnode);
// 	ft_lstadd_back(&(*parnode)->block_node->redirs_lst, redirlst);
	
// 	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
// 	return (*parnode);
// }





void *fill_blocknode_cmdarray(t_dlist **toklst, t_par_node **parnode)
{
	t_tok_node	*toknode;
	char		**arraytrav;
	int			a;

	// se o block_node ainda não tenha sido inicializado.
	if(!(*parnode)->block_node)
		intit_block_node(parnode, toklst);
	
	arraytrav = (*parnode)->block_node->cmd_arr;
	toknode = (t_tok_node *)(*toklst)->content;
	
	// acha o próximo index disponível para gravação
	a = 0;
	while(arraytrav[a])
		a++;

	// copia o valor do toknode para o index 
	arraytrav[a] = ft_strdup(toknode->value);

	// apaga o atual/primeiro node de toklst.
	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	
	//valor de retorno apenas para indicar sucesso ou erro.
	return (*parnode);
}

// inicializa o block_node na primeira vez que vai ser usado.
t_block_node *intit_block_node(t_par_node **parnode, t_dlist **toklst)
{
	int	nb_of_tokens;

	/*alocando memória para o total de tokens que ainda estão em toklst. 
	assim não preciso realocar memória dinamicamente a cada nova entrada.*/
	nb_of_tokens = ft_dlstsize(*toklst);
	(*parnode)->block_node = malloc(sizeof(t_block_node));
	(*parnode)->block_node->cmd_arr = ft_calloc(2 * nb_of_tokens, sizeof(char *) + 1); //REVISAR AQUI
	if (!(*parnode)->block_node || !(*parnode)->block_node->cmd_arr)
		return (NULL);
	(*parnode)->block_node->input_lst = NULL;
	(*parnode)->block_node->output_lst = NULL;
	(*parnode)->block_node->redirs_lst = NULL;

	//valor de retorno apenas para indicar sucesso ou erro.
	return ((*parnode)->block_node);
}

// para contar o número de parsenodes que serão criados.
int    count_num_parsnodes(t_dlist **toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;
	t_tok_node	*prevtok;
	int         total_parsnodes;

	trav = *toklst;
	total_parsnodes = 0;
	while (trav)
	{
		toknode = (t_tok_node *)trav->content;

		if (trav == *toklst)
			total_parsnodes++;	
		else if(is_command(toknode) && !is_command(prevtok))
			total_parsnodes++;	
		else if(is_pipe_logical_or_subshell(toknode))
			total_parsnodes++;	
		prevtok = toknode;
		trav = trav->next;
	}
	return (total_parsnodes);
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

t_par_exit	ft_check_syntax(t_dlist *parlst)
{
	if(!operators_are_supported(parlst))
	 	return (P_OPERATORS_NOT_SUPPORTED);
	if (!redirects_are_complete(parlst))
		return (P_INCOMPLETE_REDIRS);
	// if (pipe_at_invalid_position(parlst))
	// 	return (ERROR1);
	// if (and_or_at_invalid_positions(parlst))
	// 	return (ERROR1);
	// if (empty_parentheses(parlst))
	// 	return (ERROR1);
	return(P_SUCCESS);
}

t_par_exit	operators_are_supported(t_dlist *toklst)
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

				return (P_OPERATORS_NOT_SUPPORTED);
		}
		trav = trav->next;
	}
	return(P_SUCCESS);

}

t_par_exit	redirects_are_complete(t_dlist *toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;
	t_tok_node	*next;	
	
	trav = toklst;
	while(trav)
	{
		toknode = (t_tok_node *)trav->content;	
		// se o token é um operador de redirecionamento...
		if(is_redir(toknode)) 				
		{
			// ... ele não pode ser último token...
			if(!trav->next)
				return (P_ERROR3); 			
			next = (t_tok_node *)trav->next->content;
			// ... e precisa ser seguido de word.
			if(!next || next->oper != WORD)
				return (P_ERROR3);
		}
		trav = trav->next;
	}
	return(P_SUCCESS);
}







bool is_redir(t_tok_node *toknode)
{
	if (toknode->oper == IN_R	|| toknode->oper ==  OUT_R	||  
		toknode->oper == HDC_R	|| toknode->oper ==  APPD_R)
		return (true);
	//separando os mandatórios do bônus para melhor visualização.
	if (toknode->oper == WILD_R	|| toknode->oper ==  ERROR_R ||  
		toknode->oper == HSTR_R	|| toknode->oper ==  OERR_R)
		return (true);
	return (false);
}

// Mesmo sendo curta, ajuda a padronização e leitura.
bool is_word(t_tok_node *toknode)
{
	if (toknode->oper == WORD)
		return (true);
	return (false);
}

// Verifica se é um word ou um redirect
bool is_command(t_tok_node *toknode)
{
	if (toknode->oper == WORD)
		return (true);	
	if (toknode->oper == IN_R	|| toknode->oper ==  OUT_R	||  
		toknode->oper == HDC_R	|| toknode->oper ==  APPD_R)
		return (true);
	//separando os mandatórios do bônus para melhor visualização.
	if (toknode->oper == WILD_R	|| toknode->oper ==  ERROR_R ||  
		toknode->oper == HSTR_R	|| toknode->oper ==  OERR_R)
		return (true);
	return (false);
}


bool	is_pipe_logical_or_subshell(t_tok_node *toknode)
{
	if (toknode->oper == PIPE_O || toknode->oper == GSTART_O	||
		toknode->oper == GEND_O	|| toknode->oper == AND_O		||
		toknode->oper == OR_O)
		return(true);
	return (false);
}



void *ft_parsing_error(t_par_exit exit)
{
	int		err;
	char	*msg;

	err = errno;

	if (exit == P_INCOMPLETE_REDIRS);
	{
		err = 2;
		
	}

	msg = strerror(errno);
	
	




	return (NULL);


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

	a = 1;
	trav = *parslst;
	while(trav)
	{
		par = (t_par_node *)trav->content;

		ft_printf("\n----- Node %i-----\n", a);
		ft_printf("oper\t%i (", par->oper);
		ft_print_oper_par(par->oper);
		ft_printf(")\n");
		ft_printf("cmd\t%i\n", par->block_index);
		if (par->block_node)
		{
			if(par->block_node->cmd_arr)
			{
				ft_printf("cmdarray: {");
				ft_debug_print_array_of_strings_line(par->block_node->cmd_arr, STDOUT_FILENO);
				ft_printf("}\n");
			}
			else
				ft_printf("cmdarray: NULL\n");

			if(par->block_node->input_lst)
			{
				ft_printf("input_lst: ");
				print_redir_list(par->block_node->input_lst);
				ft_printf("\n");
			}
			else
				ft_printf("input_lst: NULL\n");	

			if(par->block_node->output_lst)
			{
				ft_printf("output_lst: ");
				print_redir_list(par->block_node->output_lst);
				ft_printf("\n");
			}
			else
				ft_printf("output_lst: NULL\n");	

			if(par->block_node->redirs_lst)
			{
				ft_printf("redirs_lst: ");
				print_redir_list(par->block_node->redirs_lst);
				ft_printf("\n");
			}

			else
				ft_printf("redirs_lst: NULL\n");
			ft_printf("\n");

		}
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
	else if (oper == CMD)
		ft_printf(GREEN "CMD" RESET);	
	else if (oper == WORD)
		ft_printf(GREEN "WORD" RESET);
	else
		ft_printf("UNKNOWN_OPERATOR (%d)", oper);
}


void print_redir_list(t_list *redirs)
{
	t_list *trav;
	t_redirs_node *redir;

	trav = redirs;
	int a = 1;
	while(trav)
	{
		redir = (t_redirs_node *)trav->content;
		ft_printf("%i) " , a);		
		ft_print_oper_par(redir->type);
		ft_printf(" [%s] " , redir->name);
		if (redir->create)
			ft_printf("create = true");
		else
			ft_printf("create = false");
		trav = trav->next;
		a++;
		ft_printf(" ");

	}
}
