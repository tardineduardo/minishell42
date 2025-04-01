#include "../include/minishell.h"

/*
This function ALWAYS returns a memory-allocated string, even if it has to be 
an empty string (len = 0), when 'string' or 'envlist' are empty. NULL is 
returned only in case of an error.

It can be used for variable expansion from both heredocs and tokens, taking 
't_exp_mode mode' as a parameter. 

All memory used during ft_expand_string's execution is allocated and freed 
within the function—except for the returned string.
*/
char *ft_expand_string(char *string, t_list **envlist, t_mem **mem)
{
	char *new;
	t_exp_mem *exp;

	if (!ft_strlen(string) || !(*envlist))
	{
		new = ft_calloc(1, sizeof(char));
		return (new);
	}
	if (exp->mode == HEREDOC)
		exp->mode == ft_heredoc_normal_or_quoted(string);
	if (exp->mode == HEREDOC_NORMAL)
		return (ft_expand_string_heredoc_normal(string, envlist, mem));
	else if (exp->mode == HEREDOC_QUOTED)
		return (ft_expand_string_heredoc_quoted(string, mem));
	else if (exp->mode == TOKEN)
		return (ft_expand_string_token(string, envlist, mem));
	ft_printf("Error: invalid expansion mode\n");
	return (NULL);
}



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



char *ft_expand_string_heredoc_normal(char *s, t_list **envlist, t_mem **mem)
{
	int		i;
	int		j;
	char	*temp;
	char	*new;

	//len multiplied by two to fit max escape char edge case.
	temp = ft_calloc(2 * ft_strlen(s), sizeof(char));
	i = 0;
	j = 0;		if (s[i] != '\\' || s[i] != '\"')
	{


	}


	if (s[i] != '\'' && s[i] != '\"')
	{
		temp[j] = s[i];
		j++;
	}	
	i++;

	while (s[i])
	{


	}
	new = (ft_strdup(temp));
	free (temp);
	return (new);
}




//APAGAR ESSA FUNCAO DEPOIS SE COUBER
char *ft_expand_string_heredoc_quoted(char *s, t_list **envlist, t_mem **mem)
{
	char *new;

	new = ft_strremove_set(s, "\"\'");
	return (new);
}

















ft_match_env







