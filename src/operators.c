#include "../include/minishell.h"

void *ft_init_operators(t_tok_mem **tok)
{
	if ((*tok)->tri_operator || (*tok)->dbl_operator || (*tok)->sgl_operator)
		return (NULL);

	(*tok)->tri_operator = malloc(2 * sizeof(char *));
	(*tok)->dbl_operator = malloc(7 * sizeof(char *));
	(*tok)->sgl_operator = ft_strdup("<>|&*()");

	if (!(*tok)->tri_operator || !(*tok)->dbl_operator || !(*tok)->sgl_operator)
		return (NULL);

	(*tok)->tri_operator[0] = ft_strdup("<<<");
	(*tok)->tri_operator[1] = NULL;
	(*tok)->dbl_operator[0] = ft_strdup(">>");
	(*tok)->dbl_operator[1] = ft_strdup("<<");
	(*tok)->dbl_operator[2] = ft_strdup("&&");
	(*tok)->dbl_operator[3] = ft_strdup("||");
	(*tok)->dbl_operator[4] = ft_strdup("2>");
	(*tok)->dbl_operator[5] = ft_strdup("&>");
	(*tok)->dbl_operator[6] = NULL;
	//protect

	return (tok);
}