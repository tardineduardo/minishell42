#ifndef EXPAND_H
# define EXPAND_H

# include "../libs/libft/libft.h"
# include "minishell.h"
# include "heredoc.h"
# include "expand.h"
# include "tokenize.h"
# include "environs.h"
# include "heredoc.h"

# define CURRENT_CHAR (*exp)->raw[(*exp)->a]
# define NEXT_CHAR (*exp)->raw[(*exp)->a + 1]
# define A (*exp)->a
# define B (*exp)->b

typedef struct	s_mem t_mem;
typedef enum e_quote t_quote;

typedef enum e_delim
{
	EXPAND,
	QUOTED,
}	t_delim;

typedef enum e_exit
{
	ERROR = 0,
	BAD_SUBSTITUITION = 0,
	INIT,
	SUCCESS,
	VAR_FOUND,
	VAR_NOT_FOUND,
	EMPTY_VARLIST,
} t_exit;

typedef struct s_exp_mem
{
	int		a;
	int		b;
	char	*raw;
	char	*new;
	char	*temp;
	bool	error;
	bool	braces;
	char	*value;
	t_list	*sortedvars;
	size_t	allocated;
	t_delim	hd_mode;
	t_mode	mode;     //misturar esses dois
	t_exit	exit;
}	t_exp_mem;




void	*ft_init_exp_memory(t_mem **mem);
void	ft_clear_exp_mem(t_exp_mem **exp);


//main
char	*ft_expand(char **string, t_mode mode, t_mem **mem);
void	*start_expansion_for_mode(t_exp_mem **exp, t_mem **mem, t_mode mode);

//modes
void	*copy_to_new_str_token_mode(t_exp_mem **exp, t_mem **mem);
void	*copy_to_new_str_delim_mode(t_exp_mem **exp);
void	*copy_to_new_str_heredoc_mode(t_exp_mem **exp, t_mem **mem);

//steps
void	update_quote_flag(char *s, t_quote *quote, int index);
bool	skip_if_quote_changed(t_exp_mem **exp, t_quote *quote, t_quote *prev);
bool	process_inside_single_quotes(t_exp_mem **exp, t_quote quote);
bool	process_inside_double_quotes(t_exp_mem **exp, t_mem **mem, t_quote quote);
bool	process_unquoted_sequence(t_exp_mem **exp, t_mem **mem);
bool	handle_dollar_sign(t_exp_mem **exp, t_mem **mem);
bool	handle_backslash(t_exp_mem **exp, t_mode mode, t_quote quote);
t_exit	try_to_expand_variable(t_exp_mem **exp, t_mem **mem);

//findvar
t_exit	get_variable_value(char *dollar, char **value, t_mem **mem);
t_exit	ft_lstcopy_and_rsort_by_len(t_list *source, t_list **sorted);
void	*lst_sort_strlen(t_list **to_sort);
t_list	*lst_sort_strlen_find_lowest(t_list *head);

//realocs
t_exit	insert_var_in_string(char *insert, size_t index, t_exp_mem **exp);
t_exit	remove_var_from_string(char **s, size_t index);

//validations
t_delim	is_delim_normal_or_quoted(char *s);
bool	is_char_escaped(char *string, int a);
bool	is_closing_quote(char c, t_quote *quote);

//ops
void	copy_char_and_increment(t_exp_mem **exp);
bool	copy_char_copy_next_and_increment(t_exp_mem **exp);
bool	skip_slash_copy_next_and_increment(t_exp_mem **exp);
void	skip_char_no_copy(t_exp_mem **exp);
void	copy_value_and_increment(t_exp_mem **exp);
size_t	varlen(char *s, bool braces);

//reset
void	reset_parameters(t_mem **mem);



#endif