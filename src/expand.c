#include "../include/minishell.h"

//expansão do delimitador (apenas as aspas são tratadas)
void		ft_exp_hd_delim_copy_to_new_str(char *s, char **new);
t_exp_mode	ft_exp_hd_delim_normal_or_quoted(char *s);
char		*ft_exp_hd_delim(char *string, t_mem **mem);

//expansão do input do heredoc
char		*ft_exp_hd_input_insert_var_in_string(char **base, char *insert, size_t index, size_t len_to_replace);
void		*ft_exp_hd_input_find_variable(t_exp_mem **exp, t_mem **mem);
void		*ft_exp_hd_input_handle_dollar_sign(t_exp_mem **exp, t_mem **mem);
bool		ft_exp_hd_input_try_to_expand_variable(t_exp_mem **exp, t_mem **mem);
bool		ft_exp_hd_input_handle_backslash_end(t_exp_mem **exp);
void		*ft_exp_hd_input_copy_to_new_str(t_exp_mem **exp, t_mem **mem);
char		*ft_exp_hd_input(char *string, t_mem **mem);

//expansão do token

//funções compartilhadas
void		ft_exp_update_quote_flag(char c, t_quote *quote);
void		*ft_exp_lst_sort_strlen(t_list **head);
t_list		*ft_exp_lst_sort_strlen_find_lowest(t_list *head);







/*
▗▖ ▗▖▗▄▄▄     ▗▄▄▄ ▗▄▄▄▖▗▖   ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖ 
▐▌ ▐▌▐▌  █    ▐▌  █▐▌   ▐▌     █  ▐▛▚▞▜▌  █    █  ▐▌   ▐▌ ▐▌
▐▛▀▜▌▐▌  █    ▐▌  █▐▛▀▀▘▐▌     █  ▐▌  ▐▌  █    █  ▐▛▀▀▘▐▛▀▚▖
▐▌ ▐▌▐▙▄▄▀    ▐▙▄▄▀▐▙▄▄▖▐▙▄▄▖▗▄█▄▖▐▌  ▐▌▗▄█▄▖  █  ▐▙▄▄▖▐▌ ▐▌  */

// Essa função vai me dizer se o heredoc será literal ou com expansão, baseado
// na presença ou não de aspas. As aspas com escape precisam ser descartadas.
// A função NÃO considera a hipótese de aspas não fechadas, esse caso deve ser
// tratado antes da etapa de expansão. Ou seja, na assim é identificado um único
// caso de aspas, simples ou duplas, a função retorna o modo "HERDOC_QUOTED".
t_exp_mode	ft_exp_hd_delim_normal_or_quoted(char *s)
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
	return (HEREDOC_EXPAND);
}

void	ft_exp_hd_delim_copy_to_new_str(char *s, char **new)
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
		ft_exp_update_quote_flag(s[a], &quote);
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

char	*ft_exp_hd_delim(char *string, t_mem **mem)
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








/*
▗▖ ▗▖▗▄▄▄     ▗▄▄▄▖▗▖  ▗▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖
▐▌ ▐▌▐▌  █      █  ▐▛▚▖▐▌▐▌ ▐▌▐▌ ▐▌  █  
▐▛▀▜▌▐▌  █      █  ▐▌ ▝▜▌▐▛▀▘ ▐▌ ▐▌  █  
▐▌ ▐▌▐▙▄▄▀    ▗▄█▄▖▐▌  ▐▌▐▌   ▝▚▄▞▘  █   */

char	*ft_exp_hd_input_insert_var_in_string(char **base, char *insert, size_t index, size_t len_to_replace)
{
	char	*new;
	size_t	base_len;
	size_t	insert_len;

	if (!base || !*base || !insert)
		return (NULL);
	base_len = ft_strlen(*base);
	insert_len = ft_strlen(insert);
	new = ft_calloc(base_len - len_to_replace + insert_len + 1,
		sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, *base, index + 1);
	ft_strlcpy(new + index, insert, insert_len + 1);
	ft_strlcpy(new + index + insert_len,
	*base + index + len_to_replace,
	base_len - index - len_to_replace + 1);
	free(*base);
	*base = new;
	return (new);
}

void	*ft_exp_hd_input_find_variable(t_exp_mem **exp, t_mem **mem)
{
	t_list		*sorted;
	t_list		*trav;
	t_env_node	*node;
	size_t		len;
	char		*str;

	str = &(*exp)->raw[(*exp)->a];
	if (!str || str[0] != '$' || str[1] == '\0')
		return (NULL);
	sorted = ft_lstcopy((*mem)->environs->envlist);
	if (!sorted)
		return (NULL);
	ft_lst_sort_strlen(&sorted);
	trav = sorted;
	while (trav)
	{
		node = (t_env_node *)trav->content;
		len = ft_strlen(node->variable);
		if (ft_strncmp(&str[1], node->variable, len) == 0)
			break ;
		trav = trav->next;
	}
	if (trav)
		ft_replace_in_string(&(*exp)->raw, node->value, (*exp)->a, len + 1);
	return ((*exp)->raw);
}

void	*ft_exp_hd_input_handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
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
	return (ft_exp_hd_input_find_variable(exp, mem));
;
}

bool	ft_exp_hd_input_try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	if ((*exp)->raw[(*exp)->a] == '$' && (*exp)->raw[(*exp)->a + 1])
	{
		if (!ft_exp_hd_input_handle_dollar_sign(exp, mem))						//ERRRO NA EXPANSAO
			return (false);														//ERRRO NA EXPANSAO
		if ((*exp)->raw[(*exp)->a] == '$')										//VARIAVEL NAO ENCONTRADA
			(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];				//'$' É COPIADO PARA A NOVA STRING
		return (true);															//VARIAVEL EXPANDIDA COM SUCESSO
	}
	return (false);																//'$' NAO ENCONTRADO, OU NO FIM DA STRING
}

bool	ft_exp_hd_input_handle_backslash_end(t_exp_mem **exp)
{
	if ((*exp)->raw[(*exp)->a] == '\\' && (*exp)->raw[(*exp)->a + 1] == '\0')
	{
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);
	}
	return (false);
}

void	*ft_exp_hd_input_copy_to_new_str(t_exp_mem **exp, t_mem **mem)
{
	t_quote	quote;
	t_quote	prev;

	quote = OFF;
	while ((*exp)->raw[(*exp)->a])
	{
		prev = quote;
		ft_exp_update_quote_flag((*exp)->raw[(*exp)->a], &quote);
		if (quote != prev)
		{
			(*exp)->a++;
			continue;
		}
		if ((*exp)->hd_mode == HEREDOC_EXPAND)
		{
			if (ft_try_expand_variable(exp, mem))
				continue;
			if (ft_handle_backslash_end(exp))
				continue;
		}
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}

/* 1) AQUI ENTRA A STRING ASSIM QUE É DIGITADA NO HEREDOC. Podemos checar se
a sring deve ser tratada literalmente ou com expansão por exp->hd_mode, que é
definida em ft_expand_string_heredoc_delimiter/ft_heredoc_normal_or_quoted
na hora de tratar as aspas do delimiter. */
char	*ft_exp_hd_input(char *string, t_mem **mem)
{
	t_exp_mem	*exp;

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
	exp->new = ft_calloc(1, sizeof(char));
	/* Alocando apenas uma string vazia para new para poder passar essa
	variável para outras funções. O tamanho de new será definido depois, de
	acordo com o length do valor da variável. */
		

	/* Aqui tem um problema, pois depois que eu chamo ft_copy_to_new_input
	o valor de exp->new é exatamente igual ao de string. Pode ser um erro
	de pointers e escopo, ou talvez uma variável trocada por engano. */
	if(!ft_exp_hd_input_copy_to_new_str(&exp, mem))
		return (NULL);

	return (exp->new);;
}







/*
▗▄▄▄▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖  ▗▖
  █ ▐▌ ▐▌▐▌▗▞▘▐▌   ▐▛▚▖▐▌
  █ ▐▌ ▐▌▐▛▚▖ ▐▛▀▀▘▐▌ ▝▜▌
  █ ▝▚▄▞▘▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌ */                         
                         











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
        low = ft_find_lowest(*to_sort);
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