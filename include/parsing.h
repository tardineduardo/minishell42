/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:12:07 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/17 14:12:27 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h" 
# include "../libs/libft/libft.h"

typedef struct s_mem	t_mem;

typedef enum e_syntax
{
	ERROR1 = 0,
	ERROR2 = 0,
	ERROR3 = 0,	
	SUCCESS_P = 1,
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
	char			*syntax_error;
}					t_par_mem;

void			*ft_parsing(t_mem **mem);
void			*ft_init_par_memory(t_mem **mem);
void			ft_clear_par_mem(t_par_mem **par);
t_syntax		ft_check_syntax(t_dlist *parlst);
t_list			*ft_create_parlst(t_dlist **toklst, t_list **parlst);
int				count_num_parsnodes(t_dlist **toklst);
t_syntax		operators_are_supported(t_dlist *parlst);
t_syntax		redirects_are_complete(t_dlist *parlst);
t_par_node		*init_parnode(int a, t_par_node **parnode, t_dlist **toklst);
t_block_node	*intit_block_node(t_par_node **parnode, t_dlist **toklst);
void			*fill_blocknode_redirs(t_dlist **toklst, t_par_node **parnode);
void			*fill_blcknode_cmdarray(t_dlist **toklst, t_par_node **parnode);

//debug
void			ft_print_oper_par(t_oper oper);
void			print_debug_parsing(t_list **parslst);
void			print_redir_list(t_list *redirs);

#endif