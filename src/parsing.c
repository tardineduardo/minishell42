/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/08 14:09:29 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"

// RETORNA O POINTER PARA ROOT DE AST
void	*ft_parsing(t_mem **mem) // antiga ft_ast_create()
{
	// t_ast_node *root;
	t_par_mem	*par;
	t_tok_mem	*tok;

	//(void)root;

	tok = (*mem)->tokenize;
	par = (*mem)->parsing;

	if (!ft_check_syntax(tok->toklst))
	{
		ft_printf("invalid sintax\n");
		return (NULL);
	}
	ft_create_parlst(&tok->toklst, &par->parlst);
	
	
	// É AQUI QUE CONTINUA. parse expression está em ast.c	
	// root = parse_expression(&par->parlst);
	


	// print_ast(root, 0);
	return (mem); //trocar para rotrnar o pointer para árvore.
}

// Cria uma lista de nodes do tipo par_node (antigo org_tok)
t_list	*ft_create_parlst(t_dlist **toklst, t_list **parlst)
{
	t_list		*new;
	t_list		*lstnode;
	t_par_node	*parnode;
	int			num_parsnodes;
	int			a;

	// checa quantos nodes serão criados.
	num_parsnodes = count_num_parsnodes(toklst);
	
	//debug
	//ft_printf("num_parsnodes == %i\n\n", num_parsnodes);

	a = 0;
	// começa a preencher os nodes de parlst.	
	while(a < num_parsnodes)
	{		
		// aloca memória para os nodes de t_list e t_par_node
		parnode = malloc(sizeof(t_par_node));
		lstnode = malloc(sizeof(t_list));
		if (!parnode || !lstnode)
			return (NULL);
	
		// inicializa as informações de cada parnode a partir de toklst
		if(!init_parnode(a, &parnode, toklst))
			return (NULL);

		// adiciona o novo parnode em parlst.
		new = ft_lstnew(parnode);
		ft_lstadd_back(parlst, new);
		a++;
	}

	//debug
	//print_debug_parsing(parlst);
	return (*parlst);
}

// inicializa as informações de cada parnode a partir de toklst
t_par_node *init_parnode(int a, t_par_node **parnode, t_dlist **toklst)
{
	t_dlist		*first;
	t_tok_node	*toknode;


	/* a lógica aqui é processar node a node, sempre apagando os nodes já 
	processados. por isso sempre se retorna para o head de toklst */


	first = *toklst;
	toknode = (t_tok_node *)first->content;

	// essas são infos básicas que valem para qualquer tipo de parnode.
	(*parnode)->block_index = toknode->block_index;
	(*parnode)->block_node = NULL;

	// se não for um operador de comando.
	if(is_pipe_logical_or_subshell(toknode))
	{
		// registra o tipo de operador.
		(*parnode)->oper = toknode->oper;
		
		// apaga o primeiro node de toklst.
		ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);

		// retorna e encerra a inicialização do parnode.
		return (*parnode);
	}

	/* caso seja um node de comando (redir ou word), começa um loop infinito
	para capturar todos os nodes necessários.*/
	while(1)
	{
		first = *toklst;
		if (!first)		// todos os tokens em toklist foram consumidos
			break ;		// encerra a incialização pois acabaram os tokens
		toknode = (t_tok_node *)first->content;
		if (toknode->block_index > a || toknode->block_index == -1) // chegou ao bloco seguinte
			break ;													// encerra porque acabou bloco
		// se o token for de um operador de redirecionamento
		if (is_redir(toknode))
			fill_blocknode_redirs(toklst, parnode);
		// se o token for uma word
		else if (is_word(toknode)) 
			fill_blocknode_cmdarray(toklst, parnode);
	}
	// no parnode, o campo oper passa a ser CMD.
	(*parnode)->oper = CMD; 
	return (*parnode);
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
		redirnode->name = ft_strdup(toknode->value); 		//AQUI PRECISA PEGAR O FULL PATH AINDA!!!!!
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
	(*parnode)->block_node->cmd_arr = ft_calloc(nb_of_tokens, sizeof(char *) + 1);
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

t_syntax	ft_check_syntax(t_dlist *parlst)
{
	// if(!operators_are_supported(parlst))
	//  	return (ERROR1);
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
		// se o token é um operador de redirecionamento...
		if(is_redir(toknode)) 				
		{
			// ... ele não pode ser último token...
			if(!trav->next)
				return (ERROR1); 			
			next = (t_tok_node *)trav->next->content;
			// ... e precisa ser seguido de word.
			if(!next || next->oper != WORD)
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

		ft_printf("\n\n----- Node %i-----\n", a);
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
		ft_printf(" [%s]" , redir->name);
		if (redir->create)
			ft_printf("create = true");
		else
			ft_printf("create = false");
		trav = trav->next;
		a++;
		if(trav)
			ft_printf(" ");
	}
}

