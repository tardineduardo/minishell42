/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/30 20:03:43 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include <assert.h>						// REMOVE LATER
# include <stdlib.h>						// for malloc etc
# include <readline/readline.h>			// for readline
# include <readline/history.h>			// for history


# define CURRENT_CHAR (*exp)->raw[(*exp)->a]
# define NEXT_CHAR (*exp)->raw[(*exp)->a + 1]
# define A (*exp)->a
# define B (*exp)->b




typedef enum e_tok_exit
{
	TOK_CONTINUE,
	TOK_END,
	TOK_ERROR,
}	t_tok_exit;

typedef enum e_exit
{
	ERROR,
	SUCCESS,
	VARIABLE_FOUND,
	VARIABLE_NOT_FOUND,
	EMPTY_VARIABLE_LIST,
	BAD_SUBSTITUITION,
	NULL_E,
} t_exit;

typedef enum e_quote
{
	Q_OFF,
	Q_SINGLE,
	Q_DOUBLE,
	Q_NULL,
}	t_quote;

typedef enum e_type
{
	TOKEN,
	EXPORT,
	HEREDOC,
	DELIMITER,
}	t_exp_mode;

typedef enum e_delim
{
	EXPAND,
	QUOTED,
}	t_delim;




//------------- STRUCTS DOS NODES DE CADA ÁREA --------------

typedef struct 	s_hd_node
{
	char		*fpath;
}	t_hd_node;

typedef struct 	s_tok_node
{
	char		*tokstr;
	bool		double_quote;
	bool		single_quote;
}	t_tok_node;

typedef struct 	s_env_node
{
	char	*variable;
	char	*value;
	bool	readonly;
	bool	block_unset;
}	t_env_node;


//------------- STRUCTS DA MEMÓRIA DE CADA ÁREA --------------

typedef struct	s_tok_mem
{
	t_list		*toklst;
	char		**operators;
	t_list		*last_of_list;
	t_tok_node	*last_of_toks;
	t_list		*new;
	t_tok_node	*node;
	char		*str;
	t_quote		quote;
	char		*remain;
}	t_tok_mem;




typedef struct	s_cap_mem
{
	char		*line;
	char		*trim;
	char		*temp;
	char		*append;
}	t_cap_mem;

typedef struct s_hd_mem
{
	t_list		*list;
	t_list		*new;
	t_hd_node	*node;

	char		*delim;
	char		*filepath;
	char		*looptemp;
	char		*loopinput;

	t_exp_mode	mode;
}	t_hd_mem;

typedef struct s_env_mem
{
	t_list		*envlist;
	t_env_node	*new_node;
	char		**result;
}	t_env_mem;


typedef struct s_exp_mem
{
	int		a;
	int		b;
	size_t	new_mem_size;
	char	*raw;
	char	*new;
	bool	error;
	bool	braces;
	char	*value;
	t_exit	expansion_exit_status;
	t_list	*sortedvars;
	t_delim		hd_mode;
	t_exp_mode	mode;

}	t_exp_mem;

typedef struct s_unc_mem
{
	t_list	*unclosed_list;
}	t_unc_mem;


//------------- STRUCT PRINCPAL DE MEMÓRIA --------------

typedef struct	s_mem
{
	t_cap_mem	*capture;
	t_hd_mem	*heredoc;
	t_tok_mem	*tokenize;
	t_exp_mem	*expand;
	t_env_mem	*environs;
	t_unc_mem	*unclosed;
}	t_mem;



// main
void	ft_init_minishell_memory(t_mem **mem, char **envp);
void	*ft_readline(t_mem **mem);
char	*ft_execute(char *line, t_mem **mem);


// heredocs
char	*ft_heredoc(char *delim, t_mem **mem);
void	ft_del_heredoc_node(void *content); // needed for EXIT

//tokens
void	*ft_tokenize(char **line, t_mem **mem);
void	*ft_init_operators(t_tok_mem **tok);
void	ft_del_token_node(void *content);

//environs
void	*ft_init_environs(t_env_mem **env, char **envp);
void	ft_del_env_node(void *content);

//operators
void *ft_init_operators(t_tok_mem **tok);

// errors and exits
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);

//interective
char *ft_capture_in_interactive_mode(char *prompt);

// built-ins
void	ft_env(t_list *envlist);
void	ft_pwd(t_list **envlist);
void	ft_echo(char *line, bool flag);
void	ft_cd(t_list **envlist, char *new_path);
void	ft_exit(void);
void	ft_export(t_list **envlist, char *variable_value);
void	ft_unset(t_list **envlist, char *variable);

// built-ins helpers
void	ft_ms_env_add(t_list **envlist, char *variable_value);
void	ft_ms_env_update_export(t_list **envlist, char *variable, char *value);
void	ft_ms_env_update_cd(t_list **envlist, char *variable, char *value);






//expansão main
char	*ft_expand(char **string, t_exp_mode mode, t_mem **mem);
void	*start_expansion_for_mode(t_exp_mem **exp, t_mem **mem, t_exp_mode mode);

//expansão modes
void	*copy_to_new_str_token_mode(t_exp_mem **exp, t_mem **mem);
void	*copy_to_new_str_delim_mode(t_exp_mem **exp);
void	*copy_to_new_str_heredoc_mode(t_exp_mem **exp, t_mem **mem);

//expansão steps
void	update_quote_flag(char *s, t_quote *quote, int index);
bool	skip_if_quote_changed(t_exp_mem **exp, t_quote *quote, t_quote *prev);
bool	process_inside_single_quotes(t_exp_mem **exp, t_quote quote);
bool	process_inside_double_quotes(t_exp_mem **exp, t_mem **mem, t_quote quote);
bool	process_unquoted_sequence(t_exp_mem **exp, t_mem **mem);
bool	handle_dollar_sign(t_exp_mem **exp, t_mem **mem);
bool	handle_backslash(t_exp_mem **exp, t_exp_mode mode, t_quote quote);
t_exit	try_to_expand_variable(t_exp_mem **exp, t_mem **mem);

//expansão findvar
t_exit	get_variable_value(char *dollar, char **value, t_mem **mem);
t_exit	ft_lstcopy_and_rsort_by_len(t_list *source, t_list **sorted);
void	*lst_sort_strlen(t_list **to_sort);
t_list	*lst_sort_strlen_find_lowest(t_list *head);

//expansao realocs
t_exit	insert_var_in_string(char *insert, size_t index, t_exp_mem **exp);
t_exit	remove_var_from_string(char **s, size_t index);

//expansao validations
t_delim	is_delim_normal_or_quoted(char *s);
bool	is_char_escaped(char *string, int a);
bool	is_closing_quote(char c, t_quote *quote);

//expansao ops
void	copy_char_and_increment(t_exp_mem **exp);
bool	copy_char_copy_next_and_increment(t_exp_mem **exp);
bool	skip_slash_copy_next_and_increment(t_exp_mem **exp);
void	skip_char_no_copy(t_exp_mem **exp);
void	copy_value_and_increment(t_exp_mem **exp);
size_t varlen(char *s, bool braces);

//expansao mid reset
void	reset(t_mem **mem);







//DEBUG - REMOVER DEPOIS
void		ft_debug_list(t_list **head);

#endif