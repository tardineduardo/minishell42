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
char *ft_expand_string(char *string, t_exp_mode mode, t_list **envlist)
{
	(void)mode;
	(void)envlist;
	return (ft_strdup(string));

}




// {
// 	assert(string);		// remove later
// 	assert(envlist);	// remove later

// 	char *new;

// 	if (!ft_strlen(string) || !(*envlist))
// 	{
// 		new = ft_calloc(1, sizeof(char));
// 		return (new);
// 	}
// 	if (mode == HEREDOC)
// 		mode == ft_heredoc_normal_or_quoted
// 	else if (mode == HEREDOC_NORMAL)
// 		return (ft_expand_string_heredoc_normal(string, envlist));
// 	else if (mode == HEREDOC_QUOTED)
// 		return (ft_strdup(string));
// 	else if (mode == TOKEN)
// 		return (ft_expand_string_token(string, envlist));
// 	ft_printf("Error: invalid expansion mode");
// 	return (NULL);
// }

// char *ft_expand_string_heredoc_normal(char *string, t_list **envlist)
// {
// 	int		i;
// 	int		j;
// 	char	*temp;
// 	char	*new;

// 	//len multiplied by two to fit max escape char edge case.
// 	temp = ft_calloc(2 * ft_strlen(string), sizeof(char));
// 	i = 0;
// 	j = 0;
// 	while (string[i])
// 	{
// 		if (string[i] != '\\' || string[i] != '\"')
// 		{


// 		}

	
// 		if (string[i] != '\'' && string[i] != '\"')
// 		{
// 			temp[j] = string[i];
// 			j++;
// 		}	
// 		i++;
// 	}
// 	new = (ft_strdup(temp));
// 	free (temp);
// 	return (new);
// }















// 	int	i;

// 	i = 0;
// 	while(string[i])
// 	{
// 		if (ft_is_double_quote(string[i]) || ft_is_single_quote(string[i]))
// 			ft_handle_quote(string[i], &quote);

	
// 		if (ft_strncmp(&string[i], "${", 2) || string[i] == '$')
// 		{
// 			ft_expand_variable(&new, &string[i], &i, env);
// 			if (i == -1)
// 				return (NULL);
// 		}
// 		else if (string[i] == '\\') 
// 			{}//handle
// 		i;
// 	}
// 	return(new);
// }










