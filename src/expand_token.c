#include "../include/minishell.h"

//expansão do token
void	*ft_expt_copy_to_new_str(t_exp_mem **exp, t_mem **mem);
char	*ft_expand_token(char *string, t_mem **mem);
bool	ft_expt_handle_backslash(t_exp_mem **exp);
bool	ft_expt_is_char_escaped(char *string, char c, int a);
bool	ft_expt_is_closing_quote(char c, t_quote *quote);



/*
▗▄▄▄▖▗▄▖ ▗▖ ▗▖▗▄▄▄▖▗▖  ▗▖
  █ ▐▌ ▐▌▐▌▗▞▘▐▌   ▐▛▚▖▐▌
  █ ▐▌ ▐▌▐▛▚▖ ▐▛▀▀▘▐▌ ▝▜▌
  █ ▝▚▄▞▘▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌ */                         



void	*ft_expt_handle_dollar_sign(t_exp_mem **exp, t_mem **mem)
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


bool	ft_expt_try_to_expand_variable(t_exp_mem **exp, t_mem **mem)
{
	if (ft_expt_is_char_escaped((*exp)->raw, '$', (*exp)->a))
	{
		(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (false);
	}
	if ((*exp)->raw[(*exp)->a] == '$' && (*exp)->raw[(*exp)->a + 1])
	{
		if (!ft_expt_handle_dollar_sign(exp, mem))//ERRRO NA EXPANSAO
			return (false);//ERRRO NA EXPANSAO
		if ((*exp)->raw[(*exp)->a] == '$')//VARIAVEL NAO ENCONTRADA
			(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);//VARIAVEL EXPANDIDA COM SUCESSO
	}
	return (false);
}




bool	ft_expt_is_quote_escaped(char *s, int a)
{
	int	escapecount;

	if (s[a] != '\'' && s[a] != '\"')
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




bool	ft_expt_is_closing_quote(char c, t_quote *quote)
{
	if (c == '\'' && *quote == SINGLE) ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		*quote = OFF;
		return (true);
	}
	else if (c == '\"' && *quote == DOUBLE)
	{
		*quote = OFF;
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
	if (quote == SINGLE)
	{
		while (!ft_expt_is_closing_quote((*exp)->raw[(*exp)->a], &quote))
			(*exp)->new[(*exp)->b++] = (*exp)->raw[(*exp)->a++];
		return (true);
	}
	return (false);
}

bool	ft_handle_double_quote(t_exp_mem **exp, t_mem **mem, t_quote quote)
{
	if (quote == DOUBLE)
	{
		if (ft_expt_try_to_expand_variable(exp, mem))
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
	if (ft_expt_try_to_expand_variable(exp, mem))
		return (true);
	if (ft_expt_handle_backslash(exp))
		return (true);
	return (false);
}






void	*ft_expt_copy_to_new_str(t_exp_mem **exp, t_mem **mem)
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
	exp->new = ft_calloc(100, sizeof(char));
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

































