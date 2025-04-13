#include "../include/minishell.h"

//funções compartilhadas
void		*ft_exp_lst_sort_strlen(t_list **head);
t_list		*ft_exp_lst_sort_strlen_find_lowest(t_list *head);
void		ft_reset_exp(t_mem **mem);
void	*ft_exp_find_variable(t_exp_mem **exp, t_mem **mem);
char	*ft_exp_remove_var_from_string(char **s, size_t index);
bool	ft_expt_is_char_escaped(char *string, char c, int a);
void	ft_exp_update_quote_flag_escaped(char *s, t_quote *quote, int index);




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
	else
		ft_exp_remove_var_from_string(&(*exp)->raw, (*exp)->a);	
	ft_lstclear(&(*exp)->sortedvars, NULL);	
	return ((*exp)->raw);
}


bool	ft_expt_is_char_escaped(char *string, char c, int a)
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
	if(!ft_expt_is_char_escaped(s, c, index))
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
;



}
