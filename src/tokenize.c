#include "../include/minishell.h"

void		*ft_tokenize_error(char *message, t_tok_mem **tok);
t_tok_exit	ft_nodesplit(t_list **head, t_tok_mem **tok);
t_tok_exit	ft_detach_node(t_tok_mem **tok, int token_limit);
int			ft_find_token_limit(char *str, t_tok_mem **tok);
bool		ft_is_tri_operator(char *str, t_tok_mem **tok);
bool		ft_is_dbl_operator(char *str, t_tok_mem **tok);
bool 		ft_is_sgl_operator(char *str, t_tok_mem **tok);
bool		ft_is_quote(char *str);
int			ft_find_quote_limit(char *str);
int			ft_find_word_limit(char *str);
void		ft_tok_node_free(void *content);
void		ft_debug_list(t_list **head);





void	*ft_tokenize(char *line, t_tok_mem **tok, t_mem **mem) // esse é o melhor?
{
	t_tok_exit	tok_exit;

	(void)mem;
	(*tok)->node = malloc(sizeof(t_tok_node));
	if (!(*tok)->node)
		return (ft_tokenize_error("ft_tokenize: malloc error\n", tok));
	(*tok)->node->tokstr = ft_strtrim(line, " \t");
	(*tok)->new = ft_lstnew((*tok)->node);
	//protect
	ft_lstadd_back(&(*tok)->toklst, (*tok)->new);
	while (1)
	{
		ft_debug_list(&(*tok)->toklst);
		tok_exit = ft_nodesplit(&(*tok)->toklst, tok);
		if (tok_exit == ERR)
			return (ft_tokenize_error("ft_tokenize: split error\n", tok));
		if (tok_exit == END)
			break ;
	}
//	ft_debug_list(&(*tok)->toklst);
	return ((*tok)->toklst);
}


t_tok_exit	ft_nodesplit(t_list **head, t_tok_mem **tok)
{
	int			token_limit;
	t_tok_exit	detach_exit;

	(*tok)->last_of_list = ft_lstlast(*head);
	(*tok)->last_of_toks = (t_tok_node *)(*tok)->last_of_list->content;
	(*tok)->str = ft_strdup((*tok)->last_of_toks->tokstr);
	//protect

	//(*tok)->str = ft_strtrim_overwrite((*tok)->str, " \t");
	token_limit = ft_find_token_limit((*tok)->str, tok);

	detach_exit = ft_detach_node(tok, token_limit);
	if (detach_exit == ERR)
		return (ERR);
	if (detach_exit == END)
		return (END);
	return (SUC);
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
	(*tok)->last_of_toks->tokstr = new_string1;
	(*tok)->node = malloc(sizeof(t_tok_node));
	(*tok)->node->tokstr = new_string2;
	(*tok)->new = ft_lstnew((*tok)->node);
	ft_lstadd_back(&(*tok)->toklst, (*tok)->new);
	return (SUC);
}



//CHECAR ANTES, EM READLINE, SE ASPAS TEM LOGICA
int	ft_find_token_limit(char *str, t_tok_mem **tok)
{
	int	i;

	i = 0;
	while(str[i] && !ft_isspace(str[i]))
	{
		if (ft_is_tri_operator(&str[i], tok))
			return (i + 3);
		else if (ft_is_dbl_operator(&str[i], tok))
			return (i + 2);
		else if (ft_is_sgl_operator(&str[i], tok))
			return (i + 1);
		else
			if (ft_is_sgl_operator(&str[i + 1], tok)) 
				return (i + 1);
			else
				i++;
	}
	return (i);
}


	
bool	ft_is_tri_operator(char *str, t_tok_mem **tok)
{
	int	i;

	i = 0;
	while((*tok)->tri_operator[i])
	{
		if (ft_strncmp(&str[0], (*tok)->tri_operator[i], 3) == 0)
			return (true);
		i++;
	}
	return (false);
}

bool	ft_is_dbl_operator(char *str, t_tok_mem **tok)
{
	int	i;

	i = 0;
	while((*tok)->dbl_operator[i])
	{
		if (ft_strncmp(&str[0], (*tok)->dbl_operator[i], 2) == 0)
			return (true);
		i++;
	}
	return (false);
}

bool ft_is_sgl_operator(char *str, t_tok_mem **tok)
{
	int	i;

	i = 0;
	while((*tok)->sgl_operator[i])
	{
		if (ft_strchr((*tok)->sgl_operator, str[0]))
			return (true);
		i++;
	}
	return (false);
}

bool ft_is_quote(char *str)
{
	if (str[0] == '"' || str[0] == '\'')
		return (true);
	return (false);
}

int	ft_find_quote_limit(char *str)
{
	int		i;
	char	quote;

	quote = str[0];
	i = 1;
	while (str[i] != quote)
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}

	return (i);
}

int	ft_find_word_limit(char *str)
{
	int		i;

	i = 0;
	while (str[i] != ' ' && str[i] != '\t' && str[i] != '\0')
		i++;
	return (i);
}











void	*ft_tokenize_error(char *message, t_tok_mem **tok)
{
	assert(message);
	assert(tok);

	if((*tok)->toklst)
	{
		ft_lstclear(&(*tok)->toklst, ft_tok_node_free);
		ft_free_and_null((void *)&(*tok)->toklst);
	}
	return (NULL);
}

void ft_tok_node_free(void *content)
{
	t_tok_node	*node;

	if (!content)
		return ;
	node = (t_tok_node *)content;
	if (node->tokstr)
		ft_free_and_null((void *)&node->tokstr);
	ft_free_and_null((void *)&node);
}












void *ft_init_operators(t_tok_mem **tok)
{
	if ((*tok)->tri_operator || (*tok)->dbl_operator || (*tok)->sgl_operator)
		return (NULL);

	(*tok)->tri_operator = malloc(2 * sizeof(char *));
	(*tok)->dbl_operator = malloc(7 * sizeof(char *));
	(*tok)->sgl_operator = ft_strdup("<>|&*()");

	if (!(*tok)->tri_operator || !(*tok)->dbl_operator || !(*tok)->sgl_operator)
		return (ft_tokenize_error("ft_init_operators: malloc error\n", tok));

	(*tok)->tri_operator[0] = ft_strdup("<<<");
	(*tok)->tri_operator[1] = NULL;

	(*tok)->dbl_operator[0] = ft_strdup(">>");
	(*tok)->dbl_operator[1] = ft_strdup("<<");
	(*tok)->dbl_operator[2] = ft_strdup("&&");
	(*tok)->dbl_operator[3] = ft_strdup("||");
	(*tok)->dbl_operator[4] = ft_strdup("2>");
	(*tok)->dbl_operator[5] = ft_strdup("&>");
	(*tok)->dbl_operator[6] = NULL;
	//protect

	return (tok);
}


void ft_debug_list(t_list **head)
{
	t_list *trav;

	trav = *head;
	ft_printf("HEAD --> ");

	if (!trav)
	{
		ft_printf("NULL\n");
		return ;
	}

	while (trav)
	{
		ft_printf("[%s] --> ", ((t_tok_node *)trav->content)->tokstr);
		trav = trav->next;
	}
	ft_printf("NULL\n");

}


// How Bash Tokenizes Input
// Splitting into Tokens (Left to Right)

// Bash reads the command character by character, breaking it into words and operators.
// It recognizes metacharacters (like <, >, |, &, ;, (, )) as separate tokens.
// Recognizing Operators and Redirections

// It first checks if any characters form a compound operator (>>, <<, |&, >&, &&, ||).
// If an operator is found, Bash immediately processes it as a special token.
// Example: In <<EOF, Bash sees << before looking at EOF.
// Handling Quotes and Escapes

// Single quotes ('...') prevent further tokenization inside.
// Double quotes ("...") allow variable expansion but still prevent space-based splitting.
// Backslashes (\) escape special characters.
// Processing Redirections and Pipes

// Bash applies redirections (<, >, <<, >>) before executing commands.
// It processes pipes (|) left to right, connecting commands.
// Parsing Here-Documents (<<)

// If Bash finds << WORD, it immediately looks ahead for the here-document delimiter and starts storing the body.
// Expanding Variables and Commands

// $VAR, $(command), and `command` are expanded after tokenization.
