#include "../include/expand.h"
#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/parsing.h"

/*return = NULL = erro. Eu ainda não gosto dessa versão. Se a ideia é que cada
módulo do programa seja realmente independente, cada chamada para tokenize
deveria receber uma **string e uma **t_list. a string seria tokenizada e os
os novos tokens salvos em t_list - que podia já estar preenchida ou não. 
Do jeito que está hoje, tokenize sempre preenche a mesma lista e fica segurando
a memória dos tokens em tok_mem mesmo depois que a tokenização já terminou.*/
void	*ft_tokenize(char **line, t_mem **mem)
{
	t_tok_mem	*tok;
	t_tok_exit	exit;

	tok = (*mem)->tokenize;
	tok->remain = ft_strdup(*line);
	while (1)
	{
		exit = ft_tokenize_remain(&tok->remain, &tok, mem);
		if (exit == TOK_ERROR)
			return (NULL);
		if (exit == TOK_END)
			break ;
	}
	ft_free_and_null((void *)&tok->remain);
	
	//LIMPAR WHITE TOKENS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	//RETIRAR ESSAS ESPANSOES E APAGAR FUNCOES, É SO PRA DEBUG
	//ft_expand_toklist(&tok->toklst, mem);			//DEBUG
	//ft_debug_list(&tok->toklst);					//DEBUG
	//ft_debug_indexes(&tok->toklst);					//DEBUG
	//ft_printf("\n");								//DEBUG
	
	return (mem);
}

t_tok_exit	ft_tokenize_remain(char **remain, t_tok_mem **tok, t_mem **mem)
{
	int			token_limit;
	t_tok_exit	detach_exit;

	token_limit = ft_find_token_limit((*remain), tok);
	detach_exit = ft_append_new_toknode(remain, tok, token_limit, mem);
	(*tok)->index_count += token_limit;
	if (detach_exit == TOK_ERROR)
		return (TOK_ERROR);
	if (detach_exit == TOK_END)
		return (TOK_END);
	return (TOK_CONTINUE);
}

t_tok_exit	ft_append_new_toknode(char **remain, t_tok_mem **tok, int token_limit, t_mem **mem)
{
	t_tok_node	*toknode;
	t_dlist		*append;
	char		*new_string;

	new_string = ft_substr((*remain), 0, token_limit);
	toknode = malloc(sizeof(t_tok_node));
	if (!toknode)
		return (TOK_ERROR);
	if (!ft_init_toknode(new_string, toknode, tok, mem))
		return (TOK_ERROR); //-------------------> SYNTAX ERROR 
	ft_free_and_null((void *)&new_string);
	append = ft_dlstnew(toknode);
	if (!append)
		return (TOK_ERROR);
	ft_dlstadd_back(&(*tok)->toklst, append);
	char *temp = *remain;
	*remain = ft_strdup(&(*remain)[token_limit]);
	ft_free_and_null((void *)&temp);
	(*tok)->index_count += ft_count_spaces(*remain);
	ft_strtrim_overwrite(remain, "\t ");
	if (!(*remain)[0])
		return (TOK_END);
	return (TOK_CONTINUE);
}

//essa função só está aqui para contar a posicao do token da string.
//se a posição nao vier a ser usada, pode apagar essa parte.
int	ft_count_spaces(char *s)
{
	int a;
	
	a = 0;
	while(ft_isspace(s[a]))
		a++;
	return (a);	
}

//essa funcão incorpora a maior parte da antiga org_tok do @luiscarvalhofrade

// PRECISA MAIS VALIDACOES! E SE O PROXIMO TERMO FOR OPERADOR? A SINTAXE ESTA
// SENDO VALIDADA SO NO PARSING, MAS ACHO QUE ALGO PRECISA SER FEITO AQUI.
// PROXIMO TOKEN NAO PODE SER OPERADOR

t_tok_node	*ft_init_toknode(char *newstring, t_tok_node *node, t_tok_mem **tok, t_mem **mem)
{
	node->value = ft_strdup(newstring);
	if (!node->value)
		return (NULL);
	node->oper = ft_get_oper(newstring);
	if (!process_heredoc(node, tok, mem))
		return (NULL);						//Retorna null se houver erro de sintaxe e develve o prompt					
	capture_values_for_parsing_later(newstring, node, tok);




//	if (ft_strncmp(newstring, "|", 1) == 0)

	return (node);
}


void	capture_values_for_parsing_later(char *newstring, t_tok_node *node, t_tok_mem **tok)
{
	t_tok_node *previous;

	if (ft_dlstlast((*tok)->toklst))
		previous = (t_tok_node *)(ft_dlstlast((*tok)->toklst)->content);
	else
		previous = NULL;
	if (is_a_luis_operator(newstring))
	{
		node->block_index = -1;
		if (previous && previous->block_index != -1)
			(*tok)->block_count++;
	}
	else
		node->block_index = (*tok)->block_count;	// Essa é a contagem que o Luis precisa ()
	node->index = (*tok)->index_count;				// Isso eu estou gravando, mas não estamos usando ainda.
													// É o index de onde começa o token na linha original.
	(*tok)->previous = node;
}



void	*process_heredoc(t_tok_node *node, t_tok_mem **tok, t_mem **mem)
{
	if ((*tok)->get_delimiter)
	{
		if (node->oper != WORD)
		{
			 // PREENCHER ERRO !!!!!!!!!
			return (NULL);
		}
		node->heredoc_path = ft_heredoc(node->value, mem);
		(*tok)->get_delimiter = false;
	}
	else
		node->heredoc_path = NULL;
	if (node->oper == HDC_R)
		(*tok)->get_delimiter = true;
	return (node);	
}





bool is_a_luis_operator(char *string)
{
	if (ft_strcmp("&&", string) == 0)
		return (true);
	else if (ft_strcmp("||", string) == 0)
		return (true);
	else if (ft_strcmp("(", string) == 0)
		return (true);
	else if (ft_strcmp(")", string) == 0)
		return (true);
	else if (ft_strcmp("|", string) == 0)
		return (true);
	return (false);
}


//que preguica de dividir isso em duas funcoes...
t_oper	ft_get_oper(char *value)
{
	if (ft_strcmp("&&", value) == 0)
		return (AND_O);
	else if (ft_strcmp("||", value) == 0)
		return (OR_O);
	else if (ft_strcmp("(", value) == 0)
		return (GSTART_O);
	else if (ft_strcmp(")", value) == 0)
		return (GEND_O);
	else if (ft_strcmp("|", value) == 0)
		return (PIPE_O);
	else if (ft_strcmp("&", value) == 0)
		return (BCKG_O);
	else if (ft_strcmp("<", value) == 0)
		return (IN_R);
	else if (ft_strcmp(">", value) == 0)
		return (OUT_R);
	else if (ft_strcmp(">>", value) == 0)
		return (APPD_R);
	else if (ft_strcmp("2>", value) == 0)
		return (ERROR_R);
	else if (ft_strcmp("<<", value) == 0)
		return (HDC_R);
	else if (ft_strcmp("<<<", value) == 0)
		return (HSTR_R);
	else if (ft_strcmp("*", value) == 0)
		return (WILD_R);
	else if (ft_strcmp("&>", value) == 0)
		return (OERR_R);
	return (WORD);
}

int	ft_find_token_limit(char *str, t_tok_mem **tok)
{
	int		i;
	int		operator_len;
	
	i = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			break ;
		if (ft_is_single_quote(&str[i]) || ft_is_double_quote(&str[i]))
		{
			i += ft_find_word_limit(tok, &str[i]);
			continue ;
		}
		if (ft_isspace(str[i]))
			break ;
		if (str[i] == '\\')	
			ft_tokeniztion_escape(&i);
		if (ft_isspace(str[i]))
			break ;		
		if (ft_is_operator(&str[i], tok, &operator_len))
		{
			if (i == 0)
				return (operator_len);
			return (i);
		}
		i++;
	}
	return (i);
}

void	ft_tokeniztion_escape(int *i)
{
	(*i)++;
	(*i)++;
	return ;
}

int		ft_find_word_limit(t_tok_mem **tok, char *str)
{
	int	i;

	if (str[0] == '\'')
		(*tok)->quote = Q_SINGLE;
	else if (str[0] == '\"')	
		(*tok)->quote = Q_DOUBLE;

	i = 1;
	while (str[i])
	{
		if ((ft_is_single_quote(&str[i]) && (*tok)->quote == Q_SINGLE) || (ft_is_double_quote(&str[i]) && (*tok)->quote == Q_DOUBLE))
			(*tok)->quote = Q_OFF;
		else if ((ft_is_single_quote(&str[i]) && (*tok)->quote == Q_OFF))
			(*tok)->quote = Q_SINGLE;
		else if ((ft_is_double_quote(&str[i]) && (*tok)->quote == Q_OFF))
			(*tok)->quote = Q_DOUBLE;
		else if ((ft_isspace(str[i]) || ft_is_operator((&str[i]), tok, NULL)) && (*tok)->quote == Q_OFF)
			return (i);
		i++;
	}
	return (i);
	// esse retorno nunca deverá ocorrer pois será validado se as aspas estao
	// em número par e nao intercaladas. 
}

bool	ft_is_operator(char *str, t_tok_mem **tok, int *operator_len)
{
	int		i;
	char	*curr_operator;

	i = 0;
	while ((*tok)->operators[i])
	{
		curr_operator = (*tok)->operators[i];
		if (ft_strncmp(str, curr_operator, ft_strlen(curr_operator)) == 0)
		{
			if (operator_len)
				*operator_len = ft_strlen(curr_operator);
			return (true);
		}
		i++;
	}
	return (false);
}



void	*ft_init_tok_memory(t_mem **mem)
{
	(*mem)->tokenize = malloc(sizeof(t_tok_mem));
	if (!(*mem)->tokenize)
		return (NULL);
	(*mem)->tokenize->operators = NULL;
	(*mem)->tokenize->toklst = NULL;
	(*mem)->tokenize->last_of_list = NULL;
	(*mem)->tokenize->last_of_toks = NULL;
	(*mem)->tokenize->new = NULL;
	(*mem)->tokenize->node = NULL;
	(*mem)->tokenize->str = NULL;
	(*mem)->tokenize->remain = NULL;
	(*mem)->tokenize->get_delimiter = false;	
	(*mem)->tokenize->index_count = 0;
	(*mem)->tokenize->block_count = 0;		
	(*mem)->tokenize->quote = Q_OFF;
	return((*mem)->tokenize);
}

void	ft_clear_tok_mem(t_tok_mem **tok)
{
	ft_dlstclear(&(*tok)->toklst, ft_del_token_node);
	ft_free_str_array((*tok)->operators);
	ft_free_and_null((void *)&(*tok)->str);
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
	{
		if (access(tok_node->heredoc_path, F_OK) == 0)
		{
			unlink(tok_node->heredoc_path);
			ft_free_and_null((void *)&tok_node->heredoc_path);
		}
	}
	ft_free_and_null((void *)&tok_node->value);
	ft_free_and_null((void *)&tok_node);
}



char *getop(t_tok_node *tok)
{
	if (tok->oper == AND_O)
		return("&&");
	if (tok->oper == OR_O)
		return("||");
	if (tok->oper == GSTART_O)
		return("(");
	if (tok->oper == GEND_O)
		return(")");
	if (tok->oper == PIPE_O)
		return("|");
	if (tok->oper == IN_R)
		return("<");
	if (tok->oper == OUT_R)
		return(">");
	if (tok->oper == APPD_R)
		return(">>");
	if (tok->oper == HDC_R)
		return("<<");
	if (tok->oper == WILD_R)
		return("*");
	else
		return (NULL);		
}



















































































void ft_expand_toklist(t_dlist **toklst, t_mem **mem)
{
	t_dlist	*trav;
	t_tok_node	*tok_node;

	trav = *toklst;
	while (trav)
	{
		if (!trav->content)
		{
			trav = trav->next;
			continue ;
		}
		tok_node = (t_tok_node *)trav->content;

		tok_node->value = ft_expand(&tok_node->value, TOKEN, mem);
		trav = trav->next;
	}
}

void ft_debug_list(t_dlist **head)
{
	t_dlist *trav;

	trav = *head;
	//ft_printf(GREY "HEAD -> " RESET);

	if (!trav)
	{
		ft_printf(GREY "NULL\n" RESET);
		return ;
	}

	while (trav)
	{
		char *value = ((t_tok_node *)trav->content)->value;

		ft_printf(GREY "[" RESET);

		if (ft_strcmp("<<<", value) == 0
			|| ft_strcmp(">>", value) == 0
			|| ft_strcmp("<<", value) == 0
			|| ft_strcmp("2>", value) == 0
			|| ft_strcmp("&>", value) == 0
			|| ft_strcmp(">", value) == 0
			|| ft_strcmp("<", value) == 0)
		{
			ft_printf(BRIGHT_BLUE "%s" RESET, value);
		}
		else if (ft_strcmp("|", value) == 0)
		{
			ft_printf(YELLOW "%s" RESET, value);
		}
		else if (ft_strcmp("&", value) == 0
			|| ft_strcmp("*", value) == 0
			|| ft_strcmp("&&", value) == 0
			|| ft_strcmp("||", value) == 0
			|| ft_strcmp("(", value) == 0
			|| ft_strcmp(")", value) == 0)
		{
			ft_printf(BRIGHT_MAGENTA "%s" RESET, value);
		}
		else
		{
			ft_printf(GREEN "%s" RESET, value);
		}

		ft_printf(GREY "] " RESET);
		trav = trav->next;
	}
	//ft_printf(GREY "NULL" RESET);

}



void ft_debug_indexes(t_dlist **head)
{
	t_dlist *trav;

	trav = *head;
	ft_printf("\n");


	ft_printf("TOKEN\t\tPOS\tGROUP\tTYPE\n");
	if (!trav)
	{
		ft_printf(GREY "NULL\n" RESET);
		return ;
	}

	while (trav)
	{
		t_oper nextoper;

		char *value = ((t_tok_node *)trav->content)->value;
		int index = ((t_tok_node *)trav->content)->index;
		int block =  ((t_tok_node *)trav->content)->block_index;
		t_oper oper = ((t_tok_node *)trav->content)->oper;
		if (trav->next)
			nextoper = ((t_tok_node *)trav->next->content)->oper;
		else
			nextoper = WORD;

	if (oper == IN_R 	|| oper == OUT_R	|| oper == APPD_R	|| HDC_R == IN_R ||
		oper == HSTR_R	|| oper == WILD_R	|| oper == ERROR_R	|| oper == OERR_R)
			ft_printf(BRIGHT_BLUE "%s" RESET, value);
		
		else if (oper == PIPE_O)
			ft_printf(YELLOW "%s" RESET, value);
		else if (oper == AND_O || oper == OR_O || oper == GSTART_O ||
				oper == GEND_O || oper == BCKG_O)
			ft_printf(BRIGHT_MAGENTA "%s" RESET, value);
		else
			ft_printf(GREEN "%s" RESET, value);

		ft_printf(GREY " " RESET);

		ft_printf("\t\t");

		ft_printf(RED "%i " RESET, index);
		ft_printf("\t");
		ft_printf("%i ", block);
		ft_printf("\t");		
		ft_print_oper(oper);

		if(((t_tok_node *)trav->content)->heredoc_path)
		{
			ft_printf("\t");		
			ft_printf("%s", ((t_tok_node *)trav->content)->heredoc_path);
		}

		ft_printf("\n");
		(void)nextoper;
		trav = trav->next;
	}



}



void	ft_print_oper(t_oper oper)
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