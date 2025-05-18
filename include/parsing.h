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

int		ft_parsing(t_mem **mem);
void	*ft_init_par_memory(t_mem **mem);
void	ft_clear_par_mem(t_par_mem **par);
bool	ft_check_syntax(t_dlist *parlst, t_par_mem **par);
t_list	*ft_create_parlst(t_dlist **toklst, t_list **parlst, t_par_mem **par);
int		count_num_parsnodes(t_dlist **toklst);
void	*operators_are_supported(t_dlist *parlst, t_par_mem **par);
void	*subshell_opers_are_correct(t_dlist *toklst, t_par_mem **par);
void	*redirects_are_complete(t_dlist *parlst, t_par_mem **par);
void	*logic_opers_are_correct(t_dlist *toklst, t_par_mem **par);
void	*pipe_opers_are_correct(t_dlist *toklst, t_par_mem **par);
t_par_node	*init_pnd(int a, t_par_node **pnd, t_dlist **toklst, t_par_mem **par);
t_block_node	*intit_bnd(t_par_node **pnd, t_dlist **toklst, t_par_mem **par);
void	*fill_bnode_redir(t_dlist **toklst, t_par_node **pnd, t_par_mem **par);
void	*fill_bnode_cmdsa(t_dlist **toklst, t_par_node **pnd, t_par_mem **par);
void	*ft_par_syscall_error(t_par_mem **par, char *ftname);
void	*ft_par_syntax_error(int st_err, char *str, t_par_mem **par);


void	ft_clear_par_mem(t_par_mem **par);
void	ft_del_par_node(void *content);
void	ft_del_redir_node(void *content);

//debug
// void	ft_print_oper_par(t_oper oper);
// void	print_debug_parsing(t_list **parslst);
// void	print_redir_list(t_list *redirs);
#endif