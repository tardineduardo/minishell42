#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/parsing.h"

void	ft_tokeniztion_escape(int *i)
{
	(*i)++;
	(*i)++;
	return ;
}


char	*getop(t_tok_node *tok)
{
	if (tok->oper == AND_O)
		return ("&&");
	if (tok->oper == OR_O)
		return ("||");
	if (tok->oper == GSTART_O)
		return ("(");
	if (tok->oper == GEND_O)
		return (")");
	if (tok->oper == PIPE_O)
		return ("|");
	if (tok->oper == IN_R)
		return ("<");
	if (tok->oper == OUT_R)
		return (">");
	if (tok->oper == APPD_R)
		return (">>");
	if (tok->oper == HDC_R)
		return ("<<");
	if (tok->oper == WILD_R)
		return ("*");
	else
		return (NULL);
}
