// arrasa luis


#include "../include/minishell.h"


char *ft_run_command(char *line)
{
	t_list	**heredocs;
	char	*delimiter;

	delimiter = NULL;
	heredocs = NULL;

	if (ft_strnstr(line, "<<", ft_strlen(line)))
	{
		delimiter = ft_strtrim(line, " <");
		ft_hc_capture(delimiter, heredocs);
	}
	if (ft_strcmp(line, "exit") == 0)
		exit(0);


	if (delimiter)
		free(delimiter);
	
	
	
	
	return (NULL);
}

