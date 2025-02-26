#include "../include/minishell.h"







//Depois eu vou expandir essa função para pegar heredoc e continuar a preencher 
//o comando se a linha terminar com |, seguindo a lógica do flowchart
char *ft_capture_command(void)
{
	char	*command_cap;
	char	*trimmed_cap;
	char	*temp_cap;

	command_cap = readline("Minishell > ");
	if (!command_cap)
		return (NULL);
	while(1)
	{
		//capture_heredocs
		trimmed_cap = ft_strtrim(command_cap, " \t");    // será que eu deveria trim todos os invisible chars?
		if (trimmed_cap[ft_strlen(trimmed_cap) - 1] == '|')
		{
			free(trimmed_cap);
			temp_cap = command_cap;
			command_cap = ft_strjoin(command_cap, (readline("> ")));			
			if (!command_cap)
			{
				free(temp_cap);
				return (NULL);
			}
			free(temp_cap);
			continue ;
		}
		free(trimmed_cap);
		break ;
	}
	add_history(command_cap);
	return (command_cap);
}
