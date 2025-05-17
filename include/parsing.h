#ifndef PARSING_H
# define PARSING_H

# include "minishell.h" 
# include "../libs/libft/libft.h"

typedef struct	s_mem t_mem;

typedef enum e_par_exit
{
	P_INCOMPLETE_REDIRS,
	P_OPERATORS_NOT_SUPPORTED,
	P_MALLOC_ERROR,
	P_ERROR3 = 0,	
	P_SUCCESS = 1,
}	t_par_exit;


typedef enum e_oper
{
	CMD		= -1,
	AND_O	= 0, 
	OR_O,
	GSTART_O,
	GEND_O,
	PIPE_O,
	WORD,
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

typedef struct s_par_node // s_ork_tok
{
	//char			*value; 		// nao precisamos disso.
	t_oper			oper; 			// t_oper em vez de int, mas os valores iguais
	int				block_index;	// esse é int cmd
	t_block_node	*block_node;
}	t_par_node;

typedef struct s_par_mem
{
	t_list		*lstnode;
	t_par_node	*parnode;
	t_list		*parlst;
	char		*error_message;
	int			error_number;

}				t_par_mem;

void		*ft_init_par_memory(t_mem **mem);
void		ft_clear_par_mem(t_par_mem **par);
t_par_exit	ft_check_syntax(t_dlist *parlst);
t_list		*ft_create_parlst(t_dlist **toklst, t_list **parlst, t_par_mem **par);
int			count_num_parsnodes(t_dlist **toklst);
t_par_exit	operators_are_supported(t_dlist *parlst);
t_par_exit	redirects_are_complete(t_dlist *parlst);

t_par_node *init_parnode(int a, t_par_mem **par, t_dlist **toklst);
t_block_node	*intit_block_node(t_par_node **parnode, t_dlist **toklst);
void			*fill_blocknode_redirs(t_dlist **toklst, t_par_node **parnode);
void			*fill_blocknode_cmdarray(t_dlist **toklst, t_par_node **parnode);
int	extract_command(int a, t_par_node **parnode, t_dlist **toklst);





int		ft_parsing(t_mem **mem);


//debug
void	ft_print_oper_par(t_oper oper);
void print_debug_parsing(t_list **parslst);
void print_redir_list(t_list *redirs);



#endif