#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"
# include "parsing.h"

typedef struct s_mem	t_mem;

typedef enum e_tok_exit
{
	TOK_CONTINUE,
	TOK_END,
	TOK_ERROR,
}	t_tok_exit;

typedef struct s_tok_node
{
	char		*value;
	bool		double_quote;
	bool		single_quote;
	int			block_index;
	size_t		index;
	t_oper		oper;
	char		*heredoc_path;
}	t_tok_node;

typedef struct s_tok_mem
{
	t_dlist		*toklst;
	char		**operators;
	t_list		*last_of_list;
	t_tok_node	*last_of_toks;
	t_list		*new;
	t_tok_node	*node;
	t_tok_node	*previous;
	char		*str;
	t_quote		quote;
	char		*remain;
	size_t		block_count;
	size_t		index_count;
	int			errnmb;
	bool		get_delimiter;
}	t_tok_mem;

void		*ft_init_tok_memory(t_mem **mem);
void		ft_clear_tok_mem(t_tok_mem **tok);
int	ft_tokenize(char **line, t_mem **mem);
int			ft_count_spaces(char *s);
int			ft_find_token_limit(char *str, t_tok_mem **tok);
int			ft_find_word_limit(t_tok_mem **tok, char *str);
bool		ft_is_oper_token(char *str, t_tok_mem **tok, int *op_len);
bool		ft_is_operator_parsing(char *string);
void		ft_del_token_node(void *content);
void		ft_expand_toklist(t_dlist **toklst, t_mem **mem);
void		ft_tokeniztion_escape(int *i);
t_tok_node	*ft_init_tknd(char *newstr, t_tok_node *node, t_tok_mem **tok);
t_tok_node	*ft_init_heredoc(t_tok_node *node, t_tok_mem **tok, t_mem **mem);
t_tok_exit	ft_tokenize_remain(char **remain, t_tok_mem **tok);
t_tok_exit	ft_append_tknde(char **remain, t_tok_mem **tok,
				int token_limit);
t_oper		ft_get_oper(char *value);
void		*ft_init_operators(t_tok_mem **tok);
void		ft_del_token_node(void *content);
void		*process_heredoc(t_tok_node *node, t_tok_mem **tok, t_mem **mem);
void		capture_values_for_parsing_later(char *newstr, t_tok_node *node,
				t_tok_mem **tok);
bool		ft_check_success(t_tok_mem **tok);

//needed later for parsing 
bool		is_word(t_tok_node *toknode);
bool		is_redir(t_tok_node *toknode);
bool		is_pipe_logical_subshell(t_tok_node *toknode);
bool		is_command(t_tok_node *toknode);
char		*getop(t_tok_node *tok);

int	ft_capture_heredocs(t_tok_mem **tok, t_mem **mem);


int			ft_check_syntax(t_dlist *toklst, t_tok_mem **tok);
int			ft_tok_syntax_error(int st_err, char *str, t_tok_mem **tok);
bool		operators_are_supported(t_dlist *trav, t_tok_mem **tok);
bool		redirects_are_complete(t_dlist *trav, t_tok_mem **tok);
bool		subshell_opers_are_correct(t_dlist *trav, t_tok_mem **tok);
bool		logic_opers_are_correct(t_dlist *trav, t_tok_mem **tok);
bool		pipe_opers_are_correct(t_dlist *trav, t_tok_mem **tok);

#endif