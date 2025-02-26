#include "../include/minishell.h"

//Depois eu vou expandir essa função para pegar heredoc e continuar a preencher 
//o comando se a linha terminar com |, seguindo a lógica do flowchart
char *ft_capture_command(void)
{
	char	*command;

	command = readline("Minishell > ");
	if (!command)
		return (NULL);
	add_history(command);
	return (command);
}
