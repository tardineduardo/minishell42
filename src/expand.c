#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/expand.h"
#include "../include/parsing.h"
#include "../include/environs.h"
#include "../include/readline.h"
                                                 
/*
""
88,dPYba,,adPYba,   ,adPPYYba,  88  8b,dPPYba,   
88P'   "88"    "8a  ""     `Y8  88  88P'   `"8a  
88      88      88  ,adPPPPP88  88  88       88  
88      88      88  88,    ,88  88  88       88  
88      88      88  `"8bbdP"Y8  88  88       88  
*/				 

// return NULL = erro. O tipo de erro fica registrado em exp->exit
char	*ft_expand(char **string, t_mode mode, t_mem **mem)
{
	t_exp_mem	*exp;
	
	if (!*string)
		return (NULL);
	if (!ft_strlen(*string))
		return (ft_strdup(""));
	exp = (*mem)->expand;
	exp->mode = mode;
	exp->raw = ft_strdup(*string);
	exp->new = ft_calloc(ft_strlen(*string) + 1, sizeof(char));
	exp->allocated = (ft_strlen(*string) + 1) * sizeof(char);
	if (!exp->new)
		return (NULL);
	if(!start_expansion_for_mode(&exp, mem, mode))
		return (NULL);
	exp->temp = *string;
	*string = ft_strdup(exp->new);
	ft_free_and_null((void *)&exp->temp);
	if (!*string)
	{
		reset_parameters(mem);
		return (NULL);
	}
	reset_parameters(mem);
	return (*string);
}

void	*start_expansion_for_mode(t_exp_mem **exp, t_mem **mem, t_mode mode)
{
	if (mode == TOKEN)
		return(copy_to_new_str_token_mode(exp, mem));
	if (mode == DELIMITER)
		return(copy_to_new_str_delim_mode(exp));
	if (mode == HEREDOC)
		return(copy_to_new_str_heredoc_mode(exp, mem));
	return (NULL);
}

/*
                                                                     
                                          88                         
                                          88                         
                                          88                         
88,dPYba,,adPYba,    ,adPPYba,    ,adPPYb,88   ,adPPYba,  ,adPPYba,  
88P'   "88"    "8a  a8"     "8a  a8"    `Y88  a8P_____88  I8[    ""  
88      88      88  8b       d8  8b       88  8PP"""""""   `"Y8ba,   
88      88      88  "8a,   ,a8"  "8a,   ,d88  "8b,   ,aa  aa    ]8I  
88      88      88   `"YbbdP"'    `"8bbdP"Y8   `"Ybbd8"'  `"YbbdP"'  
                                                                     
                                                                     
*/
void	*copy_to_new_str_token_mode(t_exp_mem **exp, t_mem **mem)
{
	t_quote	quote;
	t_quote	prev;

	quote = Q_OFF;
	while (CURRENT_CHAR)
	{
		if ((*exp)->error)
			return (NULL);
		prev = quote;
		update_quote_flag((*exp)->raw, &quote, (*exp)->a);
		if (skip_if_quote_changed(exp, &quote, &prev))
			continue;
		if (process_inside_single_quotes(exp, quote))
			continue;
		if (process_inside_double_quotes(exp, mem, quote))
			continue;
		if (process_unquoted_sequence(exp, mem))
			continue;
		copy_char_and_increment(exp);
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}


void	*copy_to_new_str_delim_mode(t_exp_mem **exp)
{
	t_quote	quote;
	t_quote	prev;
	int *a;
	char *s;

	a = &(*exp)->a;
	s = (*exp)->raw;
	quote = Q_OFF;
	while (CURRENT_CHAR)
	{
		if (ft_isquote(s[*a]) && !is_char_escaped(s, *a))
			(*exp)->hd_mode = QUOTED;
		prev = quote;
		update_quote_flag((*exp)->raw, &quote, (*exp)->a);
		if (skip_if_quote_changed(exp, &quote, &prev))
			continue;
		copy_char_and_increment(exp);
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}


void	*copy_to_new_str_heredoc_mode(t_exp_mem **exp, t_mem **mem)
{
	while (CURRENT_CHAR)
	{
		if ((*exp)->error)
			return (NULL);
		if ((*exp)->hd_mode == EXPAND)
		{
			if (handle_dollar_sign(exp, mem))
				continue;
			if (handle_backslash(exp, HEREDOC, Q_NULL))
				continue;
		}
		copy_char_and_increment(exp);
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}


/*                                                                    
                                                       
                                                       
             ,d                                        
             88                                        
,adPPYba,  MM88MMM  ,adPPYba,  8b,dPPYba,   ,adPPYba,  
I8[    ""    88    a8P_____88  88P'    "8a  I8[    ""  
 `"Y8ba,     88    8PP"""""""  88       d8   `"Y8ba,   
aa    ]8I    88,   "8b,   ,aa  88b,   ,a8"  aa    ]8I  
`"YbbdP"'    "Y888  `"Ybbd8"'  88`YbbdP"'   `"YbbdP"'  
                               88                      
                               88                      
                                                                         
*/

// used by delim, token
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

// used by delim, token
bool	skip_if_quote_changed(t_exp_mem **exp, t_quote *quote, t_quote *prev)
{
	if (*quote != *prev)
	{
		skip_char_no_copy(exp);
		return (true);
	}
	return (false);
}

// used by delim, token
bool	process_inside_single_quotes(t_exp_mem **exp, t_quote quote)
{
	if (quote == Q_SINGLE)
	{
		while (!is_closing_quote(CURRENT_CHAR, &quote))
			copy_char_and_increment(exp);
		return (true);
	}
	return (false);
}

// used by delim, token
bool	process_inside_double_quotes(t_exp_mem **exp, t_mem **mem, t_quote quote)
{
	t_mode mode;

	mode = (*exp)->mode;
	if (quote == Q_DOUBLE)
	{
		if (mode == TOKEN)
		{		
			if (handle_dollar_sign(exp, mem))
			{
				if((*exp)->braces)
					skip_char_no_copy(exp);
				return (true);
			}
			if (handle_backslash(exp, TOKEN, Q_DOUBLE))
				return (true);
		}
		if (is_closing_quote(CURRENT_CHAR, &quote)) //testar isso
		{
			skip_char_no_copy(exp);
			return (true);
		}
		copy_char_and_increment(exp);
		return (true);
	}
	return (false);
}

// used by token
bool	process_unquoted_sequence(t_exp_mem **exp, t_mem **mem)
{
	if (handle_dollar_sign(exp, mem))
	{
		if((*exp)->braces)
			skip_char_no_copy(exp);
		return (true);
	}
	if (handle_backslash(exp, TOKEN, Q_OFF))
		return (true);
	return (false);
}



// used by heredoc
// used by token->process_unquoted_sequence
// used by token->process_inside_double_quotesd
bool	handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
{
	t_exit exit;

	if (CURRENT_CHAR != '$' || NEXT_CHAR == '\0' || ft_isspace(NEXT_CHAR))
		return (false);
	if (NEXT_CHAR == '{')
	{
		(*exp)->braces = true;
		skip_char_no_copy(exp);
	}
	if (NEXT_CHAR && (ft_isalnum(NEXT_CHAR) || NEXT_CHAR == '?'))
	{
		exit = try_to_expand_variable(exp, mem);
		if (!exit)
		{
			(*exp)->exit = exit;
			return (false);
		}
		if (exit == VAR_FOUND)
		{
			copy_value_and_increment(exp);
			return (true);
		}
		return (true);//AQUI É O ELSE PARA VARIABLE NOT FOUND
	}
	return (false);
}

// used by token->process_unquoted_sequence
// used by token->process_inside_double_quotesd -> NOT DELIM
// used by delim->process_unquoted_sequence - -> NOT DELIM
// used by heredoc->process_unquoted_sequence
bool	handle_backslash(t_exp_mem **exp, t_mode mode, t_quote quote)
{
	if (CURRENT_CHAR != '\\' || NEXT_CHAR == '\0')
		return (false);
	if (mode == TOKEN && quote == Q_OFF)
	{
		if (NEXT_CHAR == '\\')
			return (copy_char_copy_next_and_increment(exp));
		else
			return (skip_slash_copy_next_and_increment(exp));
	}
	if (mode == TOKEN && quote == Q_DOUBLE)
	{
		if (ft_strchr("\\\"\'$", NEXT_CHAR))
			return (skip_slash_copy_next_and_increment(exp));
	}
	if (mode == HEREDOC && (*exp)->hd_mode == EXPAND)
	{
		if (ft_strchr("\\$", NEXT_CHAR))
			return (skip_slash_copy_next_and_increment(exp));
	}
	return (false);
}


t_exit	try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	char **value;
	t_exit exit;

	value = &(*exp)->value;

	
	// if (CURRENT_CHAR != '$' || NEXT_CHAR == '?')
	// 	exit = get_error_value(&CURRENT_CHAR, value, mem);
	// else if (CURRENT_CHAR != '$' || NEXT_CHAR == '$')
	// 	exit = get_pid_value(&CURRENT_CHAR, value, mem);
	// else
	exit = get_variable_value(&CURRENT_CHAR, value, mem);

	
	if (exit == VAR_FOUND)
		return (insert_var_in_string(*value, (*exp)->a, exp));
	else if(exit == VAR_NOT_FOUND)
		return (remove_var_from_string(&(*exp)->raw, (*exp)->a));
	else if(exit == BAD_SUBSTITUITION)
		return (BAD_SUBSTITUITION);	
	else
		return (ERROR);	
}





                
/*
                                                                               
   ad88  88                        88                                          
  d8"    ""                        88                                          
  88                               88                                          
MM88MMM  88  8b,dPPYba,    ,adPPYb,88     8b       d8  ,adPPYYba,  8b,dPPYba,  
  88     88  88P'   `"8a  a8"    `Y88     `8b     d8'  ""     `Y8  88P'   "Y8  
  88     88  88       88  8b       88      `8b   d8'   ,adPPPPP88  88          
  88     88  88       88  "8a,   ,d88       `8b,d8'    88,    ,88  88          
  88     88  88       88   `"8bbdP"Y8         "8"      `"8bbdP"Y8  88          
                                                                               
                                                                               

*/


t_exit	get_variable_value(char *dollar, char **value, t_mem **mem)
{
	t_list		*trav;
	t_env_node	*node;
	t_list		**sortedvars; 

	sortedvars = malloc(sizeof(t_list *));
	*sortedvars = (*mem)->expand->sortedvars;

	if (!dollar || (dollar[0] != '$' && dollar[0] != '{') || dollar[1] == '\0')
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
		return (VAR_FOUND);
	}
	else
	{
		ft_lstclear(sortedvars, NULL);
		free(sortedvars);
		return (VAR_NOT_FOUND);
	}
}


t_exit	ft_lstcopy_and_rsort_by_len(t_list *source, t_list **sorted)
{
	t_list	*trav;
	t_list	*new_node;

	if (!source)
		return (EMPTY_VARLIST);
	if (!sorted)
		return (ERROR);

	trav = source;
	while (trav)
	{
		new_node = ft_lstnew(trav->content);
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






/*
                                                                                
                                    88  88                                      
                                    88  88                                      
                                    88  88                                      
8b,dPPYba,   ,adPPYba,  ,adPPYYba,  88  88   ,adPPYba,    ,adPPYba,  ,adPPYba,  
88P'   "Y8  a8P_____88  ""     `Y8  88  88  a8"     "8a  a8"     ""  I8[    ""  
88          8PP"""""""  ,adPPPPP88  88  88  8b       d8  8b           `"Y8ba,   
88          "8b,   ,aa  88,    ,88  88  88  "8a,   ,a8"  "8a,   ,aa  aa    ]8I  
88           `"Ybbd8"'  `"8bbdP"Y8  88  88   `"YbbdP"'    `"Ybbd8"'  `"YbbdP"'  
                                                                                
                                                                              */

//refazer com menos systems calls
t_exit	insert_var_in_string(char *insert, size_t index, t_exp_mem **exp)
{
	char *prefix;
	char *suffix;
	char *temp;
	size_t len;

	len = varlen(&CURRENT_CHAR, (*exp)->braces);
	prefix = ft_substr((*exp)->raw, 0, index);
	suffix = ft_strdup(&CURRENT_CHAR + len + 1);
	if (!prefix || !suffix)
		return (ERROR);
	temp = ft_concatenate(prefix, insert, suffix);
	if (ft_strlen(temp) + 1 > (*exp)->allocated)
	{
		(*exp)->allocated = 2 * (ft_strlen((*exp)->raw) + 1 + ft_strlen(temp) + 1);
		ft_realloc_string(&(*exp)->new, (*exp)-> allocated);
	}
	if (!temp || !(*exp)->raw)
		return (ERROR);
	free(suffix);
	free(prefix);
	ft_free_and_null((void *)&(*exp)->raw);
	(*exp)->raw = temp;
	return (VAR_FOUND);
}

t_exit	remove_var_from_string(char **s, size_t index)
{
	size_t 	a;

	a = 0;
	while (ft_isalnum((*s)[index + a + 1]))		
		a++;
	ft_strlcpy(&(*s)[index], &(*s)[index + a + 1], ft_strlen(&(*s)[index]));
	return (VAR_NOT_FOUND);
}










/*
                                                                                                            
                         88  88           88                       88                                       
                         88  ""           88                ,d     ""                                       
                         88               88                88                                              
8b       d8  ,adPPYYba,  88  88   ,adPPYb,88  ,adPPYYba,  MM88MMM  88   ,adPPYba,   8b,dPPYba,   ,adPPYba,  
`8b     d8'  ""     `Y8  88  88  a8"    `Y88  ""     `Y8    88     88  a8"     "8a  88P'   `"8a  I8[    ""  
 `8b   d8'   ,adPPPPP88  88  88  8b       88  ,adPPPPP88    88     88  8b       d8  88       88   `"Y8ba,   
  `8b,d8'    88,    ,88  88  88  "8a,   ,d88  88,    ,88    88,    88  "8a,   ,a8"  88       88  aa    ]8I  
    "8"      `"8bbdP"Y8  88  88   `"8bbdP"Y8  `"8bbdP"Y8    "Y888  88   `"YbbdP"'   88       88  `"YbbdP"'  
                                                                                                            
                                                                                                            */

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


t_delim	is_delim_normal_or_quoted(char *s)
{
	int	a;
	int	escapecount;

	a = 0;
	while (s[a])
	{
		if (a == 0 && (ft_isquote(s[a])))
			return (EXPAND);
		if (ft_isquote(s[a]))
		{
			escapecount = 0;
			while (a > 0 && s[a - 1 - escapecount] == '\\')
				escapecount++;
			if (escapecount % 2 == 0)
				return (QUOTED);
		}
		a++;
	}
	return (QUOTED);
}





/*
                                     
 ,adPPYba,   8b,dPPYba,   ,adPPYba,  
a8"     "8a  88P'    "8a  I8[    ""  
8b       d8  88       d8   `"Y8ba,   
"8a,   ,a8"  88b,   ,a8"  aa    ]8I  
 `"YbbdP"'   88`YbbdP"'   `"YbbdP"'  
             88                      
             88                      
*/

void	copy_value_and_increment(t_exp_mem **exp)
{
	size_t len;

	len = ft_strlen((*exp)->value);

	while (len > 0)
	{
		copy_char_and_increment(exp);
		len--;
	}
	ft_free_and_null((void *)&(*exp)->value);
	return ;
}


void	copy_char_and_increment(t_exp_mem **exp)
{
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->b++;
		(*exp)->a++;
	}
	return ;
}


void skip_char_no_copy(t_exp_mem **exp)
{
	(*exp)->a++;
	return ;
}


bool	skip_slash_copy_next_and_increment(t_exp_mem **exp)
{
	(*exp)->a++;
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->a++;
		(*exp)->b++;
	}
	return (true);
}


bool	copy_char_copy_next_and_increment(t_exp_mem **exp)
{
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->a++;
		(*exp)->b++;
	}
	if (CURRENT_CHAR)
	{
		(*exp)->new[(*exp)->b] = CURRENT_CHAR;
		(*exp)->a++;
		(*exp)->b++;
	}
	return (true);
}





size_t varlen(char *s, bool braces)
{
	
	size_t	i;

	if(*s == '$')
		s++;

	if(*s == '{')
		s++;		

	i = 0;
	if (braces == false)
	{
		if (s[0] == '?')
			return (1);
		while (ft_isalnum(s[i]))
			i++;
	}
	else if (braces == true)
		while ((s[i] != '}'))
			i++;
	return (i);
}




/*
                                                                                                
                    88           88                                                             
                    ""           88                                                      ,d     
                                 88                                                      88     
88,dPYba,,adPYba,   88   ,adPPYb,88     8b,dPPYba,   ,adPPYba,  ,adPPYba,   ,adPPYba,  MM88MMM  
88P'   "88"    "8a  88  a8"    `Y88     88P'   "Y8  a8P_____88  I8[    ""  a8P_____88    88     
88      88      88  88  8b       88     88          8PP"""""""   `"Y8ba,   8PP"""""""    88     
88      88      88  88  "8a,   ,d88     88          "8b,   ,aa  aa    ]8I  "8b,   ,aa    88,    
88      88      88  88   `"8bbdP"Y8     88           `"Ybbd8"'  `"YbbdP"'   `"Ybbd8"'    "Y888  
                                                                                                
                                                                                                */

void	reset_parameters(t_mem **mem)
{
	t_exp_mem *exp;

	exp = (*mem)->expand;
	exp->a = 0;
	exp->b = 0;
	ft_free_and_null((void *)&exp->new);
	ft_free_and_null((void *)&exp->raw);
	ft_free_and_null((void *)&exp->value);
	ft_free_and_null((void *)&exp->sortedvars);
	exp->exit = INIT;
	exp->error = false;
	exp->braces = false;
}

void	*ft_init_exp_memory(t_mem **mem)
{
	(*mem)->expand = malloc(sizeof(t_exp_mem));
	if (!(*mem)->expand)
		return (NULL);
	(*mem)->expand->a = 0;
	(*mem)->expand->b = 0;
	(*mem)->expand->braces = false;
	(*mem)->expand->allocated = 0;
	(*mem)->expand->hd_mode = EXPAND;
	(*mem)->expand->raw = NULL;
	(*mem)->expand->new = NULL;
	(*mem)->expand->value = NULL;
	(*mem)->expand->error = false;
	(*mem)->expand->exit = INIT;
	(*mem)->expand->sortedvars = NULL;
	return ((*mem)->expand);
}

void	ft_clear_exp_mem(t_exp_mem **exp)
{
	ft_free_and_null((void *)&(*exp)->new);
	free(*exp);
	return ;
}
