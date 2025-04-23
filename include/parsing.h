#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"  
# include "../libs/libft/libft.h"


typedef enum e_syntax
{
	ERROR1 = 0,
	ERROR2 = 0,
	ERROR3 = 0,	
	SUCCESS_P = 1,
}	t_syntax;


typedef enum e_oper
{
	WORD = -1,
	AND_O = 0, 
	OR_O,
	GROUP_START_O,
	GROUP_END_O,
	PIPE_O,
	BCKGRND_O,
	IN_R,
	OUT_R,
	APPEND_R,
	ERROR_R,
	HEREDOC_R,		
	HERESTR_R,		
	WILDCARD_R,		
	OUT_ERROR_R,
}	t_oper;

typedef struct s_par_node
{
	char		*value;
	int			position;
	t_oper		oper;
	t_cmd_node	*cmd_node;
}	t_par_node;

typedef struct s_cmd_builder
{
	t_list	*start_node;
	t_list	*end_node;
	int		num_nodes;
} 	t_cmd_builder;


typedef struct s_par_mem
{
	t_list			*parlst;
	char			*syntax_error;
}					t_par_mem;

void	*ft_init_par_memory(t_mem **mem);
void	ft_clear_par_mem(t_par_mem **par);


void		*ft_parsing(t_mem **mem);


#endif