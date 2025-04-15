#include "../include/minishell.h"

//expansão do delimitador (apenas as aspas são tratadas)
void		hd_delim_copy_to_new_str(char *s, char **new);
t_delim	hd_delim_normal_or_quoted(char *s);
char		*expand_delim(char *string, t_mem **mem);

//expansão do input do heredoc
t_exit	get_variable_value(char *dollar, char **value, t_mem **mem);
void		*hd_input_try_to_expand_variable(t_exp_mem **exp, t_mem **mem);
bool		handle_dollar_sign(t_exp_mem **exp, t_mem **mem);
void		*hd_input_copy_to_new_str(t_exp_mem **exp, t_mem **mem);
char		*expand_hd_input(char *string, t_mem **mem);






/*
▗▖ ▗▖▗▄▄▄     ▗▄▄▄ ▗▄▄▄▖▗▖   ▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖ 
▐▌ ▐▌▐▌  █    ▐▌  █▐▌   ▐▌     █  ▐▛▚▞▜▌  █    █  ▐▌   ▐▌ ▐▌
▐▛▀▜▌▐▌  █    ▐▌  █▐▛▀▀▘▐▌     █  ▐▌  ▐▌  █    █  ▐▛▀▀▘▐▛▀▚▖
▐▌ ▐▌▐▙▄▄▀    ▐▙▄▄▀▐▙▄▄▖▐▙▄▄▖▗▄█▄▖▐▌  ▐▌▗▄█▄▖  █  ▐▙▄▄▖▐▌ ▐▌  */

// Essa função vai me dizer se o heredoc será literal ou com expansão, baseado
// na presença ou não de aspas. As aspas com escape precisam ser descartadas.
// A função NÃO considera a hipótese de aspas não fechadas, esse caso deve ser
// tratado antes da etapa de expansão. Ou seja, assim que é identificado um único
// caso de aspas, simples ou duplas, a função retorna o modo "HERDOC_QUOTED".


void	hd_delim_copy_to_new_str(char *s, char **new)
{
	int		a;
	int		b;
	t_quote	quote;
	t_quote	prev;

	quote = Q_OFF;
	a = 0;
	b = 0;
	while (s[a])
	{
		prev = quote;
		update_quote_flag(s, &quote, a);
		if (quote != prev)
		{
			a++;
			continue;
		}
		(*new)[b++] = s[a++];
	}
	return ;
}










/*
▗▖ ▗▖▗▄▄▄     ▗▄▄▄▖▗▖  ▗▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖
▐▌ ▐▌▐▌  █      █  ▐▛▚▖▐▌▐▌ ▐▌▐▌ ▐▌  █  
▐▛▀▜▌▐▌  █      █  ▐▌ ▝▜▌▐▛▀▘ ▐▌ ▐▌  █  
▐▌ ▐▌▐▙▄▄▀    ▗▄█▄▖▐▌  ▐▌▐▌   ▝▚▄▞▘  █   */








bool	hd_input_handle_backslash_end(t_exp_mem **exp)
{

	return (false);
}
















