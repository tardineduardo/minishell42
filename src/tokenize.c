#include "../include/minishell.h"

t_tok_exit	ft_tokenize_remain(char **remain, t_tok_mem **tok);
t_tok_exit	ft_append_new_toknode(char **remain, t_tok_mem **tok, int token_limit);
int			ft_find_token_limit(char *str, t_tok_mem **tok);
int			ft_find_word_limit(t_tok_mem **tok, char *str);
bool		ft_is_operator(char *str, t_tok_mem **tok, int *op_len);
void		ft_del_token_node(void *content);


void	*ft_tokenize(char **line, t_mem **mem)
{
	t_tok_mem	*tok;
	t_tok_exit	exit_status;

	tok = (*mem)->tokenize;
	tok->remain = ft_strdup(*line);
	while (1)
	{
		exit_status = ft_tokenize_remain(&tok->remain, &tok);
		if (exit_status == ERROR)
			return (NULL);
		if (exit_status == END)
			break ;
	}
	ft_free_and_null((void *)&tok->remain);
	return (mem);
}



t_tok_exit	ft_tokenize_remain(char **remain, t_tok_mem **tok)
{
	int			token_limit;
	t_tok_exit	detach_exit;


	//limit = last character of the string
	token_limit = ft_find_token_limit((*remain), tok);

	detach_exit = ft_append_new_toknode(remain, tok, token_limit);
	if (detach_exit == ERROR)
		return (ERROR);
	if (detach_exit == END)
		return (END);
	return (CONTINUE);
}


t_tok_exit	ft_append_new_toknode(char **remain, t_tok_mem **tok, int token_limit)
{
	t_tok_node	*toknode;
	t_list		*append;

	char *new_string;

	new_string = ft_substr((*remain), 0, token_limit);
	
	toknode = malloc(sizeof(t_tok_node));
	if (!toknode)
		return (ERROR);
	toknode->tokstr = new_string;
	append = ft_lstnew(toknode);
	if (!append)
		return (ERROR);
	ft_lstadd_back(&(*tok)->toklst, append);


	char *temp = (*remain);
	(*remain) = ft_strdup(&(*remain)[token_limit]);
	ft_free_and_null((void *)&temp);

	

	ft_debug_list(&(*tok)->toklst);
	ft_printf(GREY " {%s}\n" RESET, *remain);


	ft_strtrim_overwrite(remain, "\t ");
	if (!(*remain)[0])
		return (END);
	return (CONTINUE);
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


void	ft_del_token_node(void *content)
{
	t_tok_node	*tok_node;

	if (!content)
		return ;

	tok_node = (t_tok_node *)content;

//	if (tok_node->tokstr)
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
	ft_printf(GREY "NULL" RESET);

}