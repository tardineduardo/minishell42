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
	GSTART_O,
	GEND_O,
	PIPE_O,
	IN_R,
	OUT_R,
	APPD_R,
	HDC_R,
	WILD_R,
	ERROR_R,
	HSTR_R,
	OERR_R,
	BCKG_O,
}	t_oper;

typedef struct s_redirs_node
{
	t_oper			type;
	char			*name;
	bool			create;
	int				err;
}					t_redirs_node;

typedef struct s_block_node // s_cmd_node
{
	char	**cmd_arr;
	t_list	*input_lst;
	t_list	*output_lst;
	t_list	*redirs_lst;
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

void		*ft_init_par_memory(t_mem **mem);
void		ft_clear_par_mem(t_par_mem **par);
t_syntax	ft_check_syntax(t_dlist *parlst);
t_list	*ft_create_parlst(t_dlist **toklst, t_list **parlst);
int			count_num_parsnodes(t_dlist **toklst);
t_syntax	operators_are_supported(t_dlist *parlst);
t_syntax	redirects_are_complete(t_dlist *parlst);

t_par_node *init_parnode(int a, t_par_node **parnode, t_dlist **toklst);
void *fill_parnode_and_redirs(t_dlist **toklst, t_par_node **parnode);





void		*ft_parsing(t_mem **mem);


//debug
void	ft_print_oper_par(t_oper oper);
void print_debug_parsing(t_list **parslst);



#endif