/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:12:07 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/17 18:50:47 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h" 
# include "../libs/libft/libft.h"

# define EIVOPERS 33
# define EIVOPERS 33
# define EIVOPERS 33
# define EIVOPERS 33
# define EIVOPERS 33
# define EIVOPERS 33
# define EIVOPERS 33
# define EIVOPERS 33


typedef struct s_mem	t_mem;

typedef enum e_syntax
{
	EIVOPERS,
	EINCRDIR,
	ERROR1,
	ERROR2,
	ERROR3,	
	SUCCESS_P,
}	t_syntax;

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

typedef struct s_par_node
{
	t_oper			oper;
	int				block_index;
	t_block_node	*block_node;
}	t_par_node;

typedef struct s_par_mem
{
	t_list			*parlst;
	char			*errmsg;
	int				errnmb;
}					t_par_mem;

int				ft_parsing(t_mem **mem);
void			*ft_init_par_memory(t_mem **mem);
void			ft_clear_par_mem(t_par_mem **par);
bool			ft_check_syntax(t_dlist *parlst, t_par_mem **par);
t_list			*ft_create_parlst(t_dlist **toklst, t_list **parlst, t_par_mem **par);
int				count_num_parsnodes(t_dlist **toklst);
void			*operators_are_supported(t_dlist *parlst, t_par_mem **par);
void			*redirects_are_complete(t_dlist *parlst, t_par_mem **par);
t_par_node		*init_parnode(int a, t_par_node **parnode, t_dlist **toklst, t_par_mem **par);
t_block_node	*intit_block_node(t_par_node **parnode, t_dlist **toklst, t_par_mem **par);
void			*fill_blocknode_redirs(t_dlist **toklst, t_par_node **parnode, t_par_mem **par);
void			*fill_blcknode_cmdarray(t_dlist **toklst, t_par_node **parnode, t_par_mem **par);
void			*ft_parsing_syscall_error(t_par_mem **par);
void			*ft_par_syntax_error(t_syntax st_err, char *str, t_par_mem **par, int a);

//debug
void			ft_print_oper_par(t_oper oper);
void			print_debug_parsing(t_list **parslst);
void			print_redir_list(t_list *redirs);

#endif