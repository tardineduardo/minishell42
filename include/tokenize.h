/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/08 14:51:37 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"
# include "parsing.h"

# define E_NO_SUPPRT 1
# define E_INVAL_OPS 2
# define E_NO_SUBSHE 1
# define NBOPERATORS 9

typedef enum e_wcexit
{
	W_SUCCESS = 0,
	W_ERROR,
	W_NO_WILD,
	W_INVALID,
	W_ENTRIES_FOUND,
}	t_wcexit;

typedef enum e_wccase
{
	PAT_STR,
	PAT_END,	
	PAT_MID,
	PAT_EDG,
	LIST_ALL,
}	t_wccase;

typedef enum e_tok_exit
{
	TOK_CONTINUE,
	TOK_END,
	TOK_ERROR,
}	t_tok_exit;

typedef struct s_tok_node
{
	char		*value;
	int			block_index;
	t_oper		oper;
	char		*heredoc_path;
}	t_tok_node;

typedef struct s_tok_mem
{
	t_dlist		*toklst;
	char		**operators;
	t_tok_node	*previous;
	t_quote		quote;
	char		*remain;
	size_t		block_count;
	int			errnmb;
	bool		get_delimiter;
}	t_tok_mem;

typedef struct s_wild_mem
{
	DIR				*folder;
	struct dirent	*item;
	t_dlist			*wildlst;
	t_dlist			*new;
	char			*str;
	char			*joined;
}	t_wild_mem;

int			ft_tokenize(char **line, t_mem **mem);
int			ft_find_token_limit(char *str, t_tok_mem **tok);
bool		ft_is_oper_token(char *str, t_tok_mem **tok, int *op_len);
void		*ft_process_heredoc(t_tok_node *node, t_tok_mem **tok, t_mem **mem);
void		ft_index_for_parsing(char *str, t_tok_node *node, t_tok_mem **tok);
t_oper		ft_get_oper(char *value);

//memory
void		*ft_init_tok_memory(t_mem **mem);
void		ft_clear_tok_mem(t_tok_mem **tok);
void		*ft_init_operators(t_tok_mem **tok);
void		ft_del_token_node(void *content);

//needed later for parsing 
bool		ft_is_word(t_tok_node *toknode);
bool		ft_is_redir(t_tok_node *toknode);
bool		ft_is_pipe_logical_subshell(t_tok_node *toknode);
bool		ft_is_command(t_tok_node *toknode);
char		*ft_getop(t_tok_node *tok);

//syntax
int			ft_check_syntax(t_dlist *toklst, t_tok_mem **tok);
int			ft_tok_syntax_error(int st_err, char *str, t_tok_mem **tok);
bool		ft_operators_are_supported(t_dlist *trav, t_tok_mem **tok);
bool		ft_redirects_are_complete(t_dlist *trav, t_tok_mem **tok);
bool		ft_subshell_opers_are_correct(t_dlist *trav, t_tok_mem **tok);
bool		ft_logic_opers_are_correct(t_dlist *trav, t_tok_mem **tok);
bool		ft_pipe_opers_are_correct(t_dlist *trav, t_tok_mem **tok);

//wildcard
int			ft_expand_wildcards(t_dlist **toklist, t_tok_mem **tkmem);
t_wcexit	ft_token_has_valid_wildcard(t_dlist *trav, t_tok_mem **tkmem);
bool		ft_is_a_wildcard_match(char *filename, char *token, t_wccase type);
t_dlist		*ft_new_toklst_node(char *filename, t_dlist *end);
t_wccase	get_token_type(char *token);

#endif