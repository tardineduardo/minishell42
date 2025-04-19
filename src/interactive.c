#include "../include/minishell.h"

char *ft_capture_in_interactive_mode(char *prompt)
{
	return(readline(prompt));
}
