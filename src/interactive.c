#include "../include/minishell.h"

char	*ft_capture_in_interactive_mode(char *prompt, char *delim)
{

	char	*prompt;


	prompt = ft_concatenate("heredoc [", delim, "] > "));


	return(readline(prompt));
}
