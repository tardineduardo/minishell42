#include "../include/minishell.h"

//expansão do token
void	*ft_expt_copy_to_new_str(t_exp_mem **exp, t_mem **mem);
char	*ft_expand_token(char *string, t_mem **mem);
bool	ft_expt_handle_backslash(t_exp_mem **exp);
bool	ft_exp_is_char_escaped(char *string, char c, int a);
bool	ft_expt_is_closing_quote(char c, t_quote *quote);
t_exp_status	ft_expt_handle_dollar_sign(t_exp_mem **exp, t_mem **mem);

//funções compartilhadas
void		*ft_exp_lst_sort_strlen(t_list **head);
t_list		*ft_exp_lst_sort_strlen_find_lowest(t_list *head);
void		ft_reset_exp(t_mem **mem);
t_exp_status	ft_exp_get_variable_value(char *dollar, char **value, t_mem **mem);
char	*ft_exp_remove_var_from_string(char **s, size_t index);
bool	ft_exp_is_char_escaped(char *string, char c, int a);
void	ft_exp_update_quote_flag_escaped(char *s, t_quote *quote, int index);

/*
▗▄▄▄▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖  ▗▖
  █ ▐▌ ▐▌▐▌▗▞▘▐▌   ▐▛▚▖▐▌
  █ ▐▌ ▐▌▐▛▚▖ ▐▛▀▀▘▐▌ ▝▜▌
  █ ▝▚▄▞▘▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌ */                         


//////////////////////// LEVAR ISSO PARA HEREDOC (Padronizar)





t_exp_status	ft_expt_handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
{
	char *value;

	//TODO HERE: OTHER $ CASES, LIKE $$, $? etc

	value = ft_exp_get_variable_value(&(*exp)->raw[(*exp)->a], &(*exp)->value, mem);
	if (value)
	{
		ft_exp_insert_var_in_string(&(*exp)->raw, value, (*exp)->a, ft_strlen(value) + 1);
		return (EXP_VAR_FOUND);
	}
	else
	{
		ft_exp_remove_var_from_string(&(*exp)->raw, (*exp)->a);
		return (EXP_VAR_NOT_FOUND);
	}	
}


bool	ft_exp_try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	if (ft_exp_is_char_escaped((*exp)->raw, '$', (*exp)->a))
	{
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (false);
	}
	if ((*exp)->raw[(*exp)->a] == '$' && (*exp)->raw[(*exp)->a + 1])
	{
		if (ft_expt_handle_dollar_sign(exp, mem) == EXP_ERROR) //ERRRO NA EXPANSAO
			return (false);//ERRRO NA EXPANSAO
		if ((*exp)->raw[(*exp)->a] == '$')//VARIAVEL NAO ENCONTRADA
			(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);//VARIAVEL EXPANDIDA COM SUCESSO
	}
	return (false);
}









bool	ft_expt_is_closing_quote(char c, t_quote *quote)
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




bool	ft_expt_handle_backslash(t_exp_mem **exp)
{
	if ((*exp)->raw[(*exp)->a] == '\\' && (*exp)->raw[(*exp)->a + 1] == '\\')
	{
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);
	}
	return (false);
}

bool	ft_skip_if_quote_changed(t_exp_mem **exp, t_quote *quote, t_quote *prev)
{
	if (*quote != *prev)
	{
		(*exp)->a++;
		return (true);
	}
	return (false);
}

bool	ft_handle_single_quote(t_exp_mem **exp, t_quote quote)
{
	if (quote == Q_SINGLE)
	{
		while (!ft_expt_is_closing_quote((*exp)->raw[(*exp)->a], &quote))
			(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);
	}
	return (false);
}

bool	ft_handle_double_quote(t_exp_mem **exp, t_mem **mem, t_quote quote)
{
	if (quote == Q_DOUBLE)
	{
		if (ft_exp_try_to_expand_variable(exp, mem))
			return (true);
		if (ft_expt_handle_backslash(exp))
			return (true);
		if (ft_expt_is_closing_quote((*exp)->raw[(*exp)->a], &quote)) //testar isso
		{
			(*exp)->a++;
			return (true);
		}
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);
	}
	return (false);
}

bool	ft_handle_not_quoted(t_exp_mem **exp, t_mem **mem)
{
	if (ft_exp_try_to_expand_variable(exp, mem))
		return (true);
	if (ft_expt_handle_backslash(exp))
		return (true);
	return (false);
}






void	*ft_expt_copy_to_new_str(t_exp_mem **exp, t_mem **mem)
{
	t_quote	quote;
	t_quote	prev;

	quote = Q_OFF;
	while ((*exp)->raw[(*exp)->a])
	{
		prev = quote;
		ft_exp_update_quote_flag_escaped((*exp)->raw, &quote, (*exp)->a);
		if (ft_skip_if_quote_changed(exp, &quote, &prev))
			continue;
		if (ft_handle_single_quote(exp, quote))
			continue;
		if (ft_handle_double_quote(exp, mem, quote))
			continue;
		if (ft_handle_not_quoted(exp, mem))
			continue;
		(*exp)->new[(*exp)->b] = (*exp)->raw[(*exp)->a];
		(*exp)->b++;
		(*exp)->a++;
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
	/* Alocando apenas uma string vazia para new para poder passar essa
	variável para outras funções. O tamanho de new será definido depois, de
	acordo com o length do valor da variável. */


	if(!ft_expt_copy_to_new_str(&exp, mem))
		return (NULL);

	toreturn = ft_strdup(exp->new);	
	//PROTECT
	ft_reset_exp(mem);
	return (toreturn);
}























/*
 ▗▄▄▖▗▖ ▗▖ ▗▄▖ ▗▄▄▖ ▗▄▄▄▖▗▄▄▄ 
▐▌   ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌  █
 ▝▀▚▖▐▛▀▜▌▐▛▀▜▌▐▛▀▚▖▐▛▀▀▘▐▌  █
▗▄▄▞▘▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▙▄▄▖▐▙▄▄▀ */                              

void	*ft_exp_lst_sort_strlen(t_list **to_sort)
{
    t_list *low;
    t_list *sorted;

    if (!to_sort || !*to_sort)
        return (NULL);

    sorted = NULL;

    while (*to_sort)
    {
        low = ft_exp_lst_sort_strlen_find_lowest(*to_sort);
        ft_lst_unlink_node(to_sort, low);
        ft_lstadd_front(&sorted, low);
    }
    *to_sort = sorted;
    return (*to_sort);
}

t_list	*ft_exp_lst_sort_strlen_find_lowest(t_list *head)
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


t_exp_status	*ft_exp_get_variable_value(char *dollar, char **value, t_mem **mem)
{
	assert(dollar);
	assert(value);
	assert(*value);	
	
	t_list		*trav;
	t_env_node	*node;
	t_list		*sortedvars;
	
	sortedvars = (*mem)->expand->sortedvars;
	if (!dollar || dollar[0] != '$' || dollar[1] == '\0')
		return (EXP_ERROR);
	sortedvars = ft_lstcopy_and_rsort_by_len((*mem)->environs->envlist, &sortedvars);
	if (!sortedvars)
		return (EXP_ERROR); //Considera que sempre haverá pelo menos uma variável de ambiente
	trav = sortedvars;
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
		return(EXP_VAR_FOUND);
	}
	else
	{
		return (EXP_VAR_NOT_FOUND);
	}
}

bool	ft_exp_is_char_escaped(char *string, char c, int a)
{
	int	escapecount;

	if (string[a] != c)
		return (false);

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


void	ft_exp_update_quote_flag_escaped(char *s, t_quote *quote, int index)
{
	char c;

	c = s[index];
	if (!ft_isquote(c))
		return ;
	if(!ft_exp_is_char_escaped(s, c, index))
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


char	*ft_exp_insert_var_in_string(char **base, char *insert, size_t index, size_t len_to_replace)
{
	char	*new;
	size_t	base_len;
	size_t	insert_len;

	if (!base || !*base || !insert)
		return (NULL);
	base_len = ft_strlen(*base);
	insert_len = ft_strlen(insert);
	new = ft_calloc(base_len - len_to_replace + insert_len + 100,
		sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, *base, index + 1);
	ft_strlcpy(new + index, insert, insert_len + 1);
	ft_strlcpy(new + index + insert_len,
	*base + index + len_to_replace,
	base_len - index - len_to_replace + 1);
	ft_free_and_null((void *)&(*base));
	*base = new;
	return (new);
}

char	*ft_exp_remove_var_from_string(char **s, size_t index)
{
	size_t 	a;

	a = 0;

	while (ft_isalnum((*s)[index + a + 1]))		
		a++;

	ft_strlcpy(&(*s)[index], &(*s)[index + a + 1], ft_strlen(&(*s)[index]));

	return (*s);
}





/*
▗▄▄▖ ▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▄▄▄▖     ▗▄▖ ▗▖  ▗▖▗▄▄▄     ▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖▗▄▄▄▖
▐▌ ▐▌▐▌   ▐▌   ▐▌     █      ▐▌ ▐▌▐▛▚▖▐▌▐▌  █    ▐▌   ▐▌ ▐▌▐▌   ▐▌   
▐▛▀▚▖▐▛▀▀▘ ▝▀▚▖▐▛▀▀▘  █      ▐▛▀▜▌▐▌ ▝▜▌▐▌  █    ▐▛▀▀▘▐▛▀▚▖▐▛▀▀▘▐▛▀▀▘
▐▌ ▐▌▐▙▄▄▖▗▄▄▞▘▐▙▄▄▖  █      ▐▌ ▐▌▐▌  ▐▌▐▙▄▄▀    ▐▌   ▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖*/

void	ft_reset_exp(t_mem **mem)
{
	t_exp_mem *exp;

	exp = (*mem)->expand;

	exp->a = 0;
	exp->b = 0;
	ft_free_and_null((void *)&exp->new);
	ft_free_and_null((void *)&exp->raw);
	ft_free_and_null((void *)&exp->value);
	ft_lstclear(&exp->sortedvars, NULL);

}






t_exp_status	*ft_lstcopy_and_rsort_by_len(t_list *source, t_list **sorted)
{
	t_list	*trav;
	t_list	*new_node;

	if (!source)
		return (EXP_EMPTY_VARS);
	if (!sorted || !*sorted)
		return (EXP_ERROR);

	trav = source;
	while (trav)
	{
		new_node = ft_lstnew(trav->content);
		if (!new_node)
		{
			ft_lstclear(&(*sorted), free);
			return (EXP_ERROR);
		}
		ft_lstadd_back(&(*sorted), new_node);
		trav = trav->next;
	}
	ft_exp_lst_sort_strlen(sorted);
	return (EXP_SUCCESS);
}












bool	ft_expt_is_quote_escaped(char *s, int a)
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















