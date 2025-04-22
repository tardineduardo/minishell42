#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"  
# include "../libs/libft/libft.h"

typedef enum e_oper
{
	WORD = -1,
	AND_O_BONUS = 0, 
	OR_O_BONUS,
	GROUP_START_O_BONUS,
	GROUP_END_O_BONUS,
	PIPE_O,
	BACKGROUND_O_EXTRA,
	IN_REDIR,
	OUT_REDIR,
	APPEND_REDIR,
	ERROR_REDIR_EXTRA,
	HEREDOC_REDIR,		
	HERESTR_REDIR_EXTRA,		
	WILDCARD_REDIR_BONUS,		
	OUT_ERROR_REDIR_EXTRA,
}	t_oper;


typedef struct s_par_node
{
	char		*value;
	int			position;
	t_oper		oper;
	t_cmd_node	*cmd_node;
}	t_par_node;

typedef struct s_par_mem
{
	t_list			*parsedlst;
}					t_par_mem;



#endif