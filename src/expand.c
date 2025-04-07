#include "../include/minishell.h"

void	ft_update_quote_flag_delimiter(char c, t_quote *quote);
void	ft_copy_to_new_delimiter(char *s, char **new);
char	*ft_expand_string_heredoc_delimiter(char *string, t_mem **mem);
void	*ft_get_env_var(char **string, t_exp_mem *exp, t_mem **mem);
void	*ft_expand(t_exp_mem **exp, t_mem **mem);
void	ft_update_quote_flag_input(char c, t_quote *quote);
void	*ft_copy_to_new_input(t_exp_mem **exp, t_mem **mem);
char	*ft_expand_string_heredoc_input(char *string, t_mem **mem);
void	*ft_lst_sort_strlen(t_list **head);
t_list	*ft_find_lowest(t_list *head);
t_exp_mode	ft_heredoc_normal_or_quoted(char *s);



// char *ft_expand_string(char *string, t_list **envlist, t_mem **mem)
// {
// 	(void)mem;
// 	(void)envlist;
// 	return(string);
// }

void	ft_update_quote_flag_delimiter(char c, t_quote *quote)
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

void	ft_copy_to_new_delimiter(char *s, char **new)
{
	int		a;
	int		b;
	t_quote	quote;
	t_quote	prev;

	quote = OFF;
	a = 0;
	b = 0;
	while (s[a])
	{
		prev = quote;
		ft_update_quote_flag_delimiter(s[a], &quote);
		if (quote != prev)
		{
			a++;
			continue;
		}
		(*new)[b++] = s[a++];
	}
	//(*new)[b] = '\0'; tirei pq uso calloc, ver se quebra
	return ;
}

char	*ft_expand_string_heredoc_delimiter(char *string, t_mem **mem)
{
	char *new;
	t_exp_mem *exp;

	exp = (*mem)->expand;
	if (!ft_strlen(string))
		return (ft_strdup(""));
	new = ft_calloc(ft_strlen(string) + 1, sizeof(char));
	if (!new)
		return (NULL);
	ft_copy_to_new_delimiter(string, &new);
	exp->hd_mode = ft_heredoc_normal_or_quoted(string);
	return (new);
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


void	*ft_increment_and_get_env_var(char **string, t_exp_mem **exp, t_mem **mem)
{
	size_t	len;
	t_list	*sorted;
	char	*variable;		

	(void)exp;
	if (*string == NULL)
		return (NULL);
	if ((*string)[1] == 0)
		return (string); //only to check exit status
	sorted = ft_lstcopy((*mem)->environs->envlist);
	if (!sorted)
		return (NULL);
	ft_lst_sort_strlen(&sorted);
	while (sorted)
	{
		len = ft_strlen(((t_env_node *)sorted->content)->variable);
		variable = ((t_env_node *)sorted->content)->variable;
		if (ft_strncmp((*string), variable, len) == 0)
			break ;
		sorted = sorted->next;
	}
	ft_insert_into_string(string, ((t_env_node *)sorted->content)->value, (*string) + len);
	return (string);
}




void	*ft_expand(t_exp_mem **exp, t_mem **mem)
{
	// int a;
	// int b;
	// char *new;
	// char *raw;

	// new = (*exp)->new;
	// raw = (*exp)->raw;
	// a = (*exp)->a;
	// b = (*exp)->b;

	// if (raw[a] == '\\' && raw[a + 1] == '\\')
	// 	return(ft_increment_and_copy_slashes(exp, mem, 2));
	// if (raw[a] == '$' && raw[a + 1] == '$')
	// 	return(ft_increment_and_get_pid(exp));
	// if (raw[a] == '$' && raw[a + 1] == '?')
	// 	return(ft_increment_get_exit_code(exp));
	return (ft_increment_and_get_env_var(&(*exp)->raw, exp, mem));
}

void	ft_update_quote_flag_input(char c, t_quote *quote)
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

void	*ft_copy_to_new_input(t_exp_mem **exp, t_mem **mem)
{
	t_quote	quote;
	t_quote	prev;

	quote = OFF;
	while ((*exp)->raw[(*exp)->a])
	{
		prev = quote;
		ft_update_quote_flag_input((*exp)->raw[(*exp)->a], &quote);
		if (quote != prev)
		{
			(*exp)->a++;
			continue;
		}
		if ((*exp)->raw[(*exp)->a] == '\\' && (*exp)->raw[(*exp)->a + 1] == '\0')
		{
			(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
			continue;
		}
		if ((*exp)->raw[(*exp)->a] == '$' || (*exp)->raw[(*exp)->a] == '\\')
		{
			if (!ft_expand(exp, mem))
				return (NULL);
			continue ;
		}
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
	}
	return ((*exp)->new);
}

char	*ft_expand_string_heredoc_input(char *string, t_mem **mem)
{
	t_exp_mem	*exp;

	exp = (*mem)->expand;
	if (!ft_strlen(string))
	 	return (ft_strdup(""));


	exp->raw = ft_strdup(string);
	exp->new = ft_calloc(ft_strlen(string) + 1, sizeof(char));
		
	ft_copy_to_new_input(&exp, mem);

	return (string);
}
















// Essa função vai me dizer se o heredoc será literal ou com expansão, baseado
// na presença ou não de aspas. As aspas com escape precisam ser descartadas.
// A função NÃO considera a hipótese de aspas não fechadas, esse caso deve ser
// tratado antes da etapa de expansão. Ou seja, na assim é identificado um único
// caso de aspas, simples ou duplas, a função retorna o modo "HERDOC_QUOTED".
t_exp_mode	ft_heredoc_normal_or_quoted(char *s)
{
	int	a;
	int	escapecount;

	a = 0;
	while (s[a])
	{
		if (a == 0 && (s[a] == '\'' || s[a] == '\"'))
			return (HEREDOC_QUOTED);
		if (s[a] == '\'' || s[a] == '\"')
		{
			escapecount = 0;
			while (a > 0 && s[a - 1 - escapecount] == '\\')
				escapecount++;
			if (escapecount % 2 == 0)
				return (HEREDOC_QUOTED);
		}
		a++;
	}
	return (HEREDOC_NORMAL);
}




void *ft_lst_sort_strlen(t_list **head)
{
    t_list *low;
    t_list *sorted;

    if (!head || !*head)
        return (NULL);

    sorted = NULL;

    while (*head)
    {
        ft_printf("\n\n\n_________________START________________\n");
        low = ft_find_lowest(*head);
        ft_lst_unlink_node(head, low);
        ft_lstadd_front(&sorted, low);
    }
    *head = sorted;
    return (head);
}

t_list *ft_find_lowest(t_list *head)
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
      
		 ft_printf("CURR:%s, LOW: %s\n", env_current->variable, env_low->variable);
		
		  if (ft_strlen(env_current->variable) < ft_strlen(env_low->variable))
          {
               lowest = current;
               env_low = (t_env_node *)lowest->content;
          }
          current = current->next;
     }
     return (lowest);
}
