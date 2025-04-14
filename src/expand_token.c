#include "../include/minishell.h"

//expansão do token
void	*copy_to_new_str(t_exp_mem **exp, t_mem **mem);
char	*ft_expand_token(char *string, t_mem **mem);
bool	handle_backslash(t_exp_mem **exp);
bool	is_char_escaped(char *string, int a);
bool	is_closing_quote(char c, t_quote *quote);
t_exit	try_to_expand_variable(t_exp_mem **exp, t_mem **mem);
t_exit	ft_lstcopy_and_rsort_by_len(t_list *source, t_list **sorted);

//funções compartilhadas
void	*lst_sort_strlen(t_list **head);
t_list	*lst_sort_strlen_find_lowest(t_list *head);
void	reset(t_mem **mem);
t_exit	get_variable_value(char *dollar, char **value, t_mem **mem);
t_exit	remove_var_from_string(char **s, size_t index);
void	update_quote_flag(char *s, t_quote *quote, int index);
void	skip_char(t_exp_mem **exp);
void	copy_char(t_exp_mem **exp);
size_t varlen(char *s);



/*
▗▄▄▄▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖  ▗▖
  █ ▐▌ ▐▌▐▌▗▞▘▐▌   ▐▛▚▖▐▌
  █ ▐▌ ▐▌▐▛▚▖ ▐▛▀▀▘▐▌ ▝▜▌
  █ ▝▚▄▞▘▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌ */                         


//////////////////////// LEVAR ISSO PARA HEREDOC (Padronizar)








t_exit	try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	char **value;
	t_exit exit;


	//TODO HERE: OTHER $ CASES, LIKE $$, $? etc

	value = &(*exp)->value;
	exit = get_variable_value(&(*exp)->raw[(*exp)->a], value, mem);
	
	if (exit == VARIABLE_FOUND)
		return (insert_var_in_string(&(*exp)->raw, *value, (*exp)->a));
	else if(exit == VARIABLE_NOT_FOUND)
		return (remove_var_from_string(&(*exp)->raw, (*exp)->a));
	else
		return (ERROR);	
}




bool	is_closing_quote(char c, t_quote *quote)
{
	if (c == '\'' && *quote == Q_SINGLE)
	{
		*quote = Q_OFF;
		return (true);
	}
	else if (c == '\"' && *quote == Q_DOUBLE)
	{
		*quote = Q_OFF;
		return (true);
	}
	return (false);
}














bool	handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
{
	t_exit exit;

	if ((*exp)->raw[(*exp)->a] != '$')
		return (false);

	if (is_char_escaped((*exp)->raw, (*exp)->a))
	{
		copy_char(exp);
		return (false);
	}
	if ((*exp)->raw[(*exp)->a + 1])
	{
		exit = try_to_expand_variable(exp, mem);

		if (exit == ERROR)	 									//ERRRO NA EXPANSAO
		{
			(*exp)->error = true;								//ERRRO NA EXPANSAO
			return (false);										//ERRRO NA EXPANSAO
		}
		return (true);
	}
	return (false);
}


bool	handle_backslash(t_exp_mem **exp)
{
	if ((*exp)->raw[(*exp)->a] == '\\' && (*exp)->raw[(*exp)->a + 1] == '\\')
	{
		copy_char(exp);
		copy_char(exp);
		return (true);
	}
	return (false);
}

bool	ft_skip_if_quote_changed(t_exp_mem **exp, t_quote *quote, t_quote *prev)
{
	if (*quote != *prev)
	{
		skip_char(exp);
		return (true);
	}
	return (false);
}

bool	handle_single_quote(t_exp_mem **exp, t_quote quote)
{
	if (quote == Q_SINGLE)
	{
		while (!is_closing_quote((*exp)->raw[(*exp)->a], &quote))
			copy_char(exp);
		return (true);
	}
	return (false);
}

bool	handle_double_quote(t_exp_mem **exp, t_mem **mem, t_quote quote)
{
	if (quote == Q_DOUBLE)
	{
		if (handle_dollar_sign(exp, mem))
			return (true);
		if (handle_backslash(exp))
			return (true);
		if (is_closing_quote((*exp)->raw[(*exp)->a], &quote)) //testar isso
		{
			skip_char(exp);
			return (true);
		}
		copy_char(exp);
		return (true);
	}
	return (false);
}

bool	handle_not_quoted(t_exp_mem **exp, t_mem **mem)
{
	if (handle_dollar_sign(exp, mem))
		return (true);
	if (handle_backslash(exp))
		return (true);
	return (false);
}






void	*copy_to_new_str(t_exp_mem **exp, t_mem **mem)
{
	t_quote	quote;
	t_quote	prev;

	quote = Q_OFF;
	while ((*exp)->raw[(*exp)->a])
	{
		if ((*exp)->error)
			return (NULL);	
		prev = quote;
		update_quote_flag((*exp)->raw, &quote, (*exp)->a);
		if (ft_skip_if_quote_changed(exp, &quote, &prev))
			continue;
		if (handle_single_quote(exp, quote))
			continue;
		if (handle_double_quote(exp, mem, quote))
			continue;
		if (handle_not_quoted(exp, mem))
			continue;
		copy_char(exp);
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}


char	*ft_expand_token(char *string, t_mem **mem)
{
	t_exp_mem	*exp;
	char		*toreturn;

	//Primeiro checo se a string é NULL, o que é um caso de erro.
	if (!string)
		return (NULL);
	
	//Depois checo se a string é vazia. Se for retorno uma string vazia.	
	if (!ft_strlen(string))
		return (ft_strdup(""));

	/* Eu vou criar as variávereis com memória alocada dentro de uma struct
	para facilitar a limpeza de memória e reduzir o número de linhas. */
	exp = (*mem)->expand;
	exp->raw = ft_strdup(string);
	exp->new = ft_calloc(ft_strlen(string) * 2 + 1, sizeof(char));
	//PROTECT



	if(!copy_to_new_str(&exp, mem))
		return (NULL);

	toreturn = ft_strdup(exp->new);	
	//PROTECT
	reset(mem);
	return (toreturn);
}























                          

void	*lst_sort_strlen(t_list **to_sort)
{
    t_list *low;
    t_list *sorted;

    if (!to_sort || !*to_sort)
        return (NULL);

    sorted = NULL;

    while (*to_sort)
    {
        low = lst_sort_strlen_find_lowest(*to_sort);
        ft_lst_unlink_node(to_sort, low);
        ft_lstadd_front(&sorted, low);
    }
    *to_sort = sorted;
    return (*to_sort);
}

t_list	*lst_sort_strlen_find_lowest(t_list *head)
{
     t_list *lowest;
     t_list *current;
     t_env_node *env_low;
     t_env_node *env_current;

     if (!head)
          return (NULL);

     lowest = head;
     env_low = (t_env_node *)lowest->content;
     current = head->next;
     while (current)
     {
          env_current = (t_env_node *)current->content;
		  if (ft_strlen(env_current->variable) < ft_strlen(env_low->variable))
          {
               lowest = current;
               env_low = (t_env_node *)lowest->content;
          }
          current = current->next;
     }
     return (lowest);
}


t_exit	get_variable_value(char *dollar, char **value, t_mem **mem)
{
	t_list		*trav;
	t_env_node	*node;
	t_list		**sortedvars; 

	sortedvars = malloc(sizeof(t_list *));
	*sortedvars = (*mem)->expand->sortedvars;

	if (!dollar || dollar[0] != '$' || dollar[1] == '\0')
		return (ERROR);

	ft_lstcopy_and_rsort_by_len((*mem)->environs->envlist, sortedvars);

	if (!*sortedvars)
		return (ERROR);

	trav = *sortedvars;
	while (trav)
	{
		node = (t_env_node *)trav->content;
		if (ft_strncmp(&dollar[1], node->variable, ft_strlen(node->variable)) == 0)
			break ;
		trav = trav->next;
	}
	if (trav)
	{
		*value = ft_strdup(node->value);

		ft_lstclear(sortedvars, NULL);
		free(sortedvars);
		
		return (VARIABLE_FOUND);
	}
	else
	{
		ft_lstclear(sortedvars, NULL);
		free(sortedvars);
		return (VARIABLE_NOT_FOUND);
	}
}


bool	is_char_escaped(char *string, int a)
{
	int	escapecount;

	if (a <= 0)
		return (false);

	escapecount = 0;
	while (a > 0)
	{
		if (string[a - 1] == '\\')
			escapecount++;
		if (escapecount % 2 == 1)
			return (true);
		a--;
	}
	return (false);

}


void	update_quote_flag(char *s, t_quote *quote, int index)
{
	char c;

	c = s[index];
	if (!ft_isquote(c))
		return ;
	if(!is_char_escaped(s, index))
	{
		if (c == '\'' && *quote == Q_OFF)
			*quote = Q_SINGLE;
		else if (c == '\"' && *quote == Q_OFF)
			*quote = Q_DOUBLE;
		else if (c == '\'' && *quote == Q_SINGLE)
			*quote = Q_OFF;
		else if (c == '\"' && *quote == Q_DOUBLE)
			*quote = Q_OFF;
	}
	return ;
}


t_exit	insert_var_in_string(char **base, char *insert, size_t index)
{
	char *prefix;
	char *suffix;
	char *new;
	size_t len;

	len = varlen(&(*base)[index + 1]);
	prefix = ft_substr(*base, 0, index);
	suffix = ft_strdup(*base + index + len + 1);

	ft_printf("prefix: %s\n", prefix);
	ft_printf("insert: %s\n", insert);
	ft_printf("suffix: %s\n", suffix);

	if (!prefix || !suffix)
		return (ERROR);

	new = ft_concatenate_var(3, prefix, insert, suffix);
	free(suffix);
	free(prefix);
	ft_free_and_null((void *)&(*base));
	*base = new;
	return (VARIABLE_FOUND);
}

t_exit	remove_var_from_string(char **s, size_t index)
{
	size_t 	a;

	a = 0;

	while (ft_isalnum((*s)[index + a + 1]))		
		a++;

	ft_strlcpy(&(*s)[index], &(*s)[index + a + 1], ft_strlen(&(*s)[index]));

	return (VARIABLE_NOT_FOUND);
}





/*
▗▄▄▖ ▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▄▄▄▖     ▗▄▖ ▗▖  ▗▖▗▄▄▄     ▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖▗▄▄▄▖
▐▌ ▐▌▐▌   ▐▌   ▐▌     █      ▐▌ ▐▌▐▛▚▖▐▌▐▌  █    ▐▌   ▐▌ ▐▌▐▌   ▐▌   
▐▛▀▚▖▐▛▀▀▘ ▝▀▚▖▐▛▀▀▘  █      ▐▛▀▜▌▐▌ ▝▜▌▐▌  █    ▐▛▀▀▘▐▛▀▚▖▐▛▀▀▘▐▛▀▀▘
▐▌ ▐▌▐▙▄▄▖▗▄▄▞▘▐▙▄▄▖  █      ▐▌ ▐▌▐▌  ▐▌▐▙▄▄▀    ▐▌   ▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖*/

void	reset(t_mem **mem)
{
	t_exp_mem *exp;

	exp = (*mem)->expand;

	exp->a = 0;
	exp->b = 0;
	ft_free_and_null((void *)&exp->new);
	ft_free_and_null((void *)&exp->raw);
	ft_free_and_null((void *)&exp->value);


	//ft_lstclear(&exp->sortedvars, NULL);

	ft_free_and_null((void *)&exp->sortedvars);
	exp->error = false;
}






t_exit	ft_lstcopy_and_rsort_by_len(t_list *source, t_list **sorted)
{
	t_list	*trav;
	t_list	*new_node;

	if (!source)
		return (EMPTY_VARIABLE_LIST);
	if (!sorted)
		return (ERROR);

	trav = source;
	while (trav)
	{
		new_node = ft_lstnew(trav->content);  //TEM QUE DAR FREE NISSO
		if (!new_node)
		{
			ft_lstclear(sorted, free);
			*sorted = NULL;
			return (ERROR);
		}
		ft_lstadd_back(sorted, new_node);
		trav = trav->next;
	}
	lst_sort_strlen(sorted);
	return (SUCCESS);
}












bool	is_quote_escaped(char *s, int a)
{
	int	escapecount;

	if (!ft_isquote(s[a]))
		return (false);
	if (a <= 0)
		return (false);

	escapecount = 0;
	while (a > 0)
	{
		if (s[a - 1] == '\\')
			escapecount++;
		if (escapecount % 2 == 1)
			return (true);
		a--;
	}
	return (false);
}














void	copy_char(t_exp_mem **exp)
{
	(*exp)->new[(*exp)->b] = (*exp)->raw[(*exp)->a];
	(*exp)->b++;
	(*exp)->a++;
	return ;
}

void skip_char(t_exp_mem **exp)
{
	(*exp)->a++;
	return ;
}

size_t varlen(char *s)
{
	size_t i;

	i = 0;
	while (ft_isalnum(s[i]))
		i++;
	return (i);
}
