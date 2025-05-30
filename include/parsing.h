/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:12:07 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/22 12:54:16 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include "../libs/libft/libft.h"

# define E_NO_SUPPRT 56
# define E_INVAL_OPS 2
# define E_NO_SUBSHE 55

typedef struct s_mem	t_mem;

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
	int				errnmb;
}					t_par_mem;

typedef struct s_ast_mem
{
	t_ast_node	*root;
}	t_ast_mem;

int				ft_parsing(t_mem **mem);
void			*ft_init_par_memory(t_mem **mem);
void			ft_clear_par_mem(t_par_mem **par);
t_list			*ft_create_parlst(t_dlist **toklst, t_list **parlst,
					t_par_mem **par);
int				count_num_parsnodes(t_dlist **toklst);
t_par_node		*init_pnd(int a, t_par_node **pnd, t_dlist **toklst,
					t_par_mem **par);
t_block_node	*init_bnd(t_par_node **pnd, t_dlist **toklst, t_par_mem **par);
void			*fill_bnode_redir(t_dlist **toklst, t_par_node **pnd,
					t_par_mem **par);
void			*fill_bnode_cmdsa(t_dlist **toklst, t_par_node **pnd,
					t_par_mem **par);
void			*ft_par_syscall_error(t_par_mem **par, char *ftname);
void			ft_clear_par_mem(t_par_mem **par);
void			ft_del_par_node(void *content);
void			ft_del_redir_node(void *content);
void			*ft_init_ast_memory(t_mem **mem);
void			ft_clear_ast_mem(t_ast_mem **ast);

//syntax

#endif