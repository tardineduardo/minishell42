#include "../include/minishell.h"

//funções compartilhadas
void		ft_exp_update_quote_flag(char c, t_quote *quote);
void		*ft_exp_lst_sort_strlen(t_list **head);
t_list		*ft_exp_lst_sort_strlen_find_lowest(t_list *head);
void		ft_reset_exp(t_mem **mem);
void	*ft_exp_find_variable(t_exp_mem **exp, t_mem **mem);




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


void	*ft_exp_find_variable(t_exp_mem **exp, t_mem **mem)
{
	t_list		*trav;
	t_env_node	*node;
	size_t		len;
	char		*str;

	str = &(*exp)->raw[(*exp)->a];
	if (!str || str[0] != '$' || str[1] == '\0')
		return (NULL);
	(*exp)->sortedvars = ft_lstcopy((*mem)->environs->envlist);
	if (!(*exp)->sortedvars)
		return (NULL);
	ft_exp_lst_sort_strlen(&(*exp)->sortedvars);
	trav = (*exp)->sortedvars;
	while (trav)
	{
		node = (t_env_node *)trav->content;
		len = ft_strlen(node->variable);
		if (ft_strncmp(&str[1], node->variable, len) == 0)
			break ;
		trav = trav->next;
	}
	if (trav)
		ft_exp_insert_var_in_string(&(*exp)->raw, node->value, (*exp)->a, len + 1);
	ft_lstclear(&(*exp)->sortedvars, NULL);	
	return ((*exp)->raw);
}




//talvez usar a outra de token, checando se quotes sao escaped
void	ft_exp_update_quote_flag(char c, t_quote *quote)
{
	if (c == '\'' && *quote == OFF)
		*quote = SINGLE;
	else if (c == '\"' && *quote == OFF)
		*quote = DOUBLE;
	else if (c == '\'' && *quote == SINGLE)
		*quote = OFF;
	else if (c == '\"' && *quote == DOUBLE)
		*quote = OFF;
}

void	ft_exp_update_quote_flag_escaped(char *c, t_quote *quote, int index)
{
	if (c[index] != '\'' && c[index] != '\"')
		return ;
	if(!ft_exp_token_is_quote_escaped(c, index))
	{
		if (*c == '\'' && *quote == OFF)
			*quote = SINGLE;
		else if (*c == '\"' && *quote == OFF)
			*quote = DOUBLE;
		else if (*c == '\'' && *quote == SINGLE)
			*quote = OFF;
		else if (*c == '\"' && *quote == DOUBLE)
			*quote = OFF;
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
;



}
