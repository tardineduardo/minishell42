#include "../include/minishell.h"

void		*ft_tokenize_error(char *message, t_tok_mem **tok);
t_tok_exit	ft_nodesplit(t_list **head, t_tok_mem **tok);
t_tok_exit	ft_detach_node(t_tok_mem **tok, int token_limit);
int			ft_find_token_limit(char *str, t_tok_mem **tok);
int			ft_find_word_limit(t_tok_mem **tok, char *str);
bool		ft_is_operator(char *str, t_tok_mem **tok, int *op_len);
void		ft_del_token_node(void *content);
void		ft_debug_list(t_list **head);
bool		ft_tok_is_end_quote(char *c, t_tok_mem **tok);
void		*ft_append_toknode(char *line, t_list **toklist);



/*

EU ME ODEIOOOOOOOOO
BURROOOOOOOOO
BURROOOOOOOOO
REESCREVER ESTA MERDA JUNTANDO NODE + STRING
(VAI FICAR COM A METADE DO TAMANHO)

*/

// t_cap_mem *cap;
// t_env_mem *env;
// t_tok_mem *tok;
// t_hd_mem *hd;

// cap = (*mem)->capture;
// env = (*mem)->environs;
// tok = (*mem)->tokenize;
// hd = (*mem)->heredoc;




void	*ft_tokenize(char *line, t_mem **mem) // esse é o melhor?
{
	t_tok_mem *tok;

	tok = (*mem)->tokenize;
	
	t_tok_exit	tok_exit;

	if (!ft_append_toknode(line, &tok->toklst))
		return (NULL);

	while (1)
	{
		ft_debug_list(&tok->toklst);
		tok_exit = ft_nodesplit(&tok->toklst, &tok);
		if (tok_exit == ERROR)
			return (NULL);
		if (tok_exit == END)
			break ;
	}
	return (mem);
}



void	*ft_append_toknode(char *line, t_list **toklist)
{
	t_tok_node *new;
	t_list 		*append;

	new = malloc(sizeof(t_tok_node));
	if (!new)
		return (NULL);
	new->tokstr = ft_strdup(line);
	append = ft_lstnew(new);
	//protect
	//protect
	ft_lstadd_back(toklist, append);
	return (toklist);
}



t_tok_exit	ft_nodesplit(t_list **head, t_tok_mem **tok)
{
	int			token_limit;
	t_tok_exit	detach_exit;

	(*tok)->last_of_list = ft_lstlast(*head);
	(*tok)->last_of_toks = (t_tok_node *)(*tok)->last_of_list->content;
	if ((*tok)->str )
		ft_free_and_null((void *)&(*tok)->str);
	(*tok)->str = ft_strdup((*tok)->last_of_toks->tokstr);
	//protect

	//limit = last character of the string
	token_limit = ft_find_token_limit((*tok)->str, tok);

	detach_exit = ft_detach_node(tok, token_limit);
	if (detach_exit == ERROR)
		return (ERROR);
	if (detach_exit == END)
		return (END);
	return (CONTINUE);
}


t_tok_exit	ft_detach_node(t_tok_mem **tok, int token_limit)
{
	char *new_string1;
	char *new_string2;

	(*tok)->last_of_toks = (t_tok_node *)ft_lstlast((*tok)->toklst)->content;
	if (ft_strlen((*tok)->last_of_toks->tokstr) == (size_t)token_limit)
		return (END);

	new_string1 = ft_substr((*tok)->last_of_toks->tokstr, 0, token_limit);
	new_string2 = ft_strdup(&(*tok)->last_of_toks->tokstr[token_limit]);
	new_string2 = ft_strtrim_overwrite(new_string2, " \t");
	
	if ((*tok)->last_of_toks->tokstr)
		ft_free_and_null((void *)&(*tok)->last_of_toks->tokstr);
	free((*tok)->last_of_toks->tokstr);
	(*tok)->last_of_toks->tokstr = new_string1;
	(*tok)->node = malloc(sizeof(t_tok_node));
	(*tok)->node->tokstr = new_string2;
	(*tok)->new = ft_lstnew((*tok)->node);
	ft_lstadd_back(&(*tok)->toklst, (*tok)->new);
	
	return (CONTINUE);
}


int	ft_find_token_limit(char *str, t_tok_mem **tok)
{
	int	i;
	int	operator_len;

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
		if (ft_is_operator(&str[i], tok, &operator_len))
		{
			if (i == 0)
				return (operator_len); // operator at start
			return (i); // operator found later, split before it
		}
		i++;
	}
	return (i);
}

int			ft_find_word_limit(t_tok_mem **tok, char *str)
{
	int	i;

	if (str[0] == '\'')
		(*tok)->quote = SINGLE;
	else if (str[0] == '\"')	
		(*tok)->quote = DOUBLE;

	i = 1;
	while (str[i])
	{
		if ((ft_is_single_quote(&str[i]) && (*tok)->quote == SINGLE) || (ft_is_double_quote(&str[i]) && (*tok)->quote == DOUBLE))
			(*tok)->quote = OFF;
		else if ((ft_is_single_quote(&str[i]) && (*tok)->quote == OFF))
			(*tok)->quote = SINGLE;
		else if ((ft_is_double_quote(&str[i]) && (*tok)->quote == OFF))
			(*tok)->quote = DOUBLE;
		else if ((ft_isspace(str[i]) || ft_is_operator((&str[i]), tok, NULL)) && (*tok)->quote == OFF)
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


void	*ft_tokenize_error(char *message, t_tok_mem **tok)
{
	assert(message);
	assert(tok);

	if((*tok)->toklst)
	{
		ft_lstclear(&(*tok)->toklst, ft_del_token_node);
		ft_free_and_null((void *)&(*tok)->toklst);
	}
	return (NULL);
}



void	ft_del_token_node(void *content)
{
	t_tok_node	*tok_node;

	if (!content)
		return ;

	tok_node = (t_tok_node *)content;

	if (tok_node->tokstr)
		ft_free_and_null((void *)&tok_node->tokstr);

	ft_free_and_null((void *)&tok_node);
}








































void ft_debug_list(t_list **head)
{
	t_list *trav;

	trav = *head;
	ft_printf(GREY "HEAD -> " RESET);

	if (!trav)
	{
		ft_printf(GREY "NULL\n" RESET);
		return ;
	}

	while (trav)
	{
		char *tokstr = ((t_tok_node *)trav->content)->tokstr;

		ft_printf(GREY "[" RESET);

		if (ft_strcmp("<<<", tokstr) == 0
			|| ft_strcmp(">>", tokstr) == 0
			|| ft_strcmp("<<", tokstr) == 0
			|| ft_strcmp("2>", tokstr) == 0
			|| ft_strcmp("&>", tokstr) == 0
			|| ft_strcmp(">", tokstr) == 0
			|| ft_strcmp("<", tokstr) == 0)
		{
			ft_printf(BRIGHT_BLUE "%s" RESET, tokstr);
		}
		else if (ft_strcmp("|", tokstr) == 0)
		{
			ft_printf(YELLOW "%s" RESET, tokstr);
		}
		else if (ft_strcmp("&", tokstr) == 0
			|| ft_strcmp("*", tokstr) == 0
			|| ft_strcmp("&&", tokstr) == 0
			|| ft_strcmp("||", tokstr) == 0
			|| ft_strcmp("(", tokstr) == 0
			|| ft_strcmp(")", tokstr) == 0)
		{
			ft_printf(BRIGHT_MAGENTA "%s" RESET, tokstr);
		}
		else
		{
			ft_printf(GREEN "%s" RESET, tokstr);
		}

		ft_printf(GREY "] -> " RESET);
		trav = trav->next;
	}
	ft_printf(GREY "NULL\n" RESET);

}