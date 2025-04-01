#include "../include/minishell.h"

void *ft_init_operators(t_tok_mem **tok)
{
	(*tok)->operators = malloc(15 * sizeof(char *));

	(*tok)->operators[0] = ft_strdup("<<<");
	(*tok)->operators[1] = ft_strdup(">>");
	(*tok)->operators[2] = ft_strdup("<<");
	(*tok)->operators[3] = ft_strdup("&&");
	(*tok)->operators[4] = ft_strdup("||");
	(*tok)->operators[5] = ft_strdup("2>");
	(*tok)->operators[6] = ft_strdup("&>");
	(*tok)->operators[7] = ft_strdup("<");
	(*tok)->operators[8] = ft_strdup(">");
	(*tok)->operators[9] = ft_strdup("|");
	(*tok)->operators[10] = ft_strdup("&");
	(*tok)->operators[11] = ft_strdup("*");
	(*tok)->operators[12] = ft_strdup("(");
	(*tok)->operators[13] = ft_strdup(")");
	(*tok)->operators[14] = NULL;

	//protect

	return (tok);
}