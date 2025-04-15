#include "../include/minishell.h"

char *ft_capture_in_interactive_mode(char *prompt)
{
	return(readline(prompt));
}



// #define PROMPT "\001\033[1;32m\002minishell$ \001\033[0m\002"

// char *line = readline(PROMPT);