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
	IN_R, 				//NAO MEXER NA ORDEM!!!!
	OUT_R,//NAO MEXER NA ORDEM!!!!
	APPEND_R,//NAO MEXER NA ORDEM!!!!
	ERROR_R,//NAO MEXER NA ORDEM!!!!
	HEREDOC_R,		//NAO MEXER NA ORDEM!!!!
	HERESTR_R,		//NAO MEXER NA ORDEM!!!!
	WILDCARD_R,		//NAO MEXER NA ORDEM!!!!
	OUT_ERROR_R,//NAO MEXER NA ORDEM!!!!
}	t_oper;


typedef struct s_redirs
{
	t_oper	type;
	char			*path;
	int				err;
}					t_redirs;


typedef struct s_block_node // s_cmd_node
{
	char	**cmd_arr;
	t_list	*input_lst;
	t_list	*output_lst;
	int		err;
}			t_block_node;


typedef struct s_par_node // s_ork_tok
{
	//char			*value; ---	nao precisamos disso.
	t_oper			oper; // --	aqui uso t_oper em vez de int, mas os valores sao os mesmos
	int				block_index; //
	t_block_node	*block_node;
}	t_par_node;




typedef struct s_par_mem
{
	t_list			*parlst;
	char			*syntax_error;
}					t_par_mem;

void	*ft_init_par_memory(t_mem **mem);
void	ft_clear_par_mem(t_par_mem **par);


void		*ft_parsing(t_mem **mem);


#endif