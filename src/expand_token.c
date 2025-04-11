#include "../include/minishell.h"

//expansão do token
void	*ft_exp_token_copy_to_new_str(t_exp_mem **exp, t_mem **mem);
char	*ft_exp_token(char *string, t_mem **mem);
bool	ft_exp_token_handle_backslash(t_exp_mem **exp);
bool	ft_exp_token_is_dollar_sign_escaped(char *s, int index);
bool	ft_exp_token_is_closing_quote(t_exp_mem **exp, t_quote *quote);






/*
▗▄▄▄▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖  ▗▖
  █ ▐▌ ▐▌▐▌▗▞▘▐▌   ▐▛▚▖▐▌
  █ ▐▌ ▐▌▐▛▚▖ ▐▛▀▀▘▐▌ ▝▜▌
  █ ▝▚▄▞▘▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌ */                         



//TODOS os outros backslashes além de \\ e do \ no final da string eu vou
// tratar antes de validar as aspas ou o dollar sign (vendo se a soma de barras
//é par ou impar.



void	*ft_exp_token_handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
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
	return (ft_exp_find_variable(exp, mem));
}


bool	ft_exp_token_try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	if (ft_exp_token_is_dollar_sign_escaped((*exp)->raw, (*exp)->a))
	{
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (false);
	}
	if ((*exp)->raw[(*exp)->a] == '$' && (*exp)->raw[(*exp)->a + 1])
	{
		if (!ft_exp_token_handle_dollar_sign(exp, mem))//ERRRO NA EXPANSAO
			return (false);//ERRRO NA EXPANSAO
		if ((*exp)->raw[(*exp)->a] == '$')//VARIAVEL NAO ENCONTRADA
			(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);//VARIAVEL EXPANDIDA COM SUCESSO
	}
	return (false);
}


bool	ft_exp_token_is_dollar_sign_escaped(char *s, int index)
{
	assert(s != NULL);

	//assert(index >= 0 && index < (int)ft_strlen(s));

	int	a;
	int	escapecount;

	// essa funçao nao checa erro, s nao pode ser NULL, index tem que ser
	// maior ou igual a 0 e menor que o length de s.
	if (index == 0)
		return (false);

	a = index;
	while (s[a])
	{
		if (s[a] == '$')
		{
			escapecount = 0;
			while (a > 0 && s[a - 1 - escapecount] == '\\')
				escapecount++;
			if (escapecount % 2 == 0)
				return (false);
		}
		a++;
	}
	return (true);
}

bool	ft_exp_token_is_closing_quote(t_exp_mem **exp, t_quote *quote)
{
	if ((*exp)->raw[(*exp)->a] == '\'' && *quote == SINGLE)
	{
		*quote = OFF;
		(*exp)->a++;
		return (true);
	}
	else if ((*exp)->raw[(*exp)->a] == '\"' && *quote == DOUBLE)
	{
		*quote = OFF;
		(*exp)->a++;
		return (true);
	}
	return (false);
}

bool	ft_exp_token_is_quote_escaped(char *s, int index)
{
	assert(s != NULL);
	assert(index >= 0 && index < (int)ft_strlen(s));

	int	a;
	int	escapecount;

	// essa funçao nao checa erro, s nao pode ser NULL, index tem que ser
	// maior ou igual a 0 e menor que o length de s.
	if (index == 0)
		return (false);

	a = index;
	while (s[a])
	{
		if (s[a] == '\'' || s[a] == '\"')
		{
			escapecount = 0;
			while (a > 0 && s[a - 1 - escapecount] == '\\')
				escapecount++;
			if (escapecount % 2 == 0)
				return (false);
		}
		a++;
	}
	return (true);
}


bool	ft_exp_token_handle_backslash(t_exp_mem **exp)
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
	if (quote == SINGLE)
	{
		if (ft_exp_token_is_closing_quote(exp, &quote))
			return (true);
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);
	}
	return (false);
}

bool	ft_handle_double_quote(t_exp_mem **exp, t_mem **mem, t_quote quote)
{
	if (quote == DOUBLE)
	{
		if (ft_exp_token_try_to_expand_variable(exp, mem))
			return (true);
		if (ft_exp_token_handle_backslash(exp))
			return (true);
		if (ft_exp_token_is_closing_quote(exp, &quote))
			return (true);
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);
	}
	return (false);
}

bool	ft_handle_not_quoted(t_exp_mem **exp, t_mem **mem)
{
	if (ft_exp_token_try_to_expand_variable(exp, mem))
		return (true);
	if (ft_exp_token_handle_backslash(exp))
		return (true);
	return (false);
}






void	*ft_exp_token_copy_to_new_str(t_exp_mem **exp, t_mem **mem)
{
	t_quote	quote;
	t_quote	prev;

	quote = OFF;
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
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
	}
	(*exp)->new[(*exp)->b] = '\0';
	return ((*exp)->new);
}


char	*ft_exp_token(char *string, t_mem **mem)
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
	exp->new = ft_calloc(100, sizeof(char));
	/* Alocando apenas uma string vazia para new para poder passar essa
	variável para outras funções. O tamanho de new será definido depois, de
	acordo com o length do valor da variável. */
		

	/* Aqui tem um problema, pois depois que eu chamo ft_copy_to_new_input
	o valor de exp->new é exatamente igual ao de string. Pode ser um erro
	de pointers e escopo, ou talvez uma variável trocada por engano. */

	printf("raw = \"%s\"\n", exp->raw);
	printf("strlen(raw) = %zu\n", strlen(exp->raw));

	if(!ft_exp_token_copy_to_new_str(&exp, mem))
		return (NULL);

	toreturn = ft_strdup(exp->new);	
	//PROTECT
	ft_reset_exp(mem);
	return (toreturn);
}

































