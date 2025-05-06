/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/06 18:47:48 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include <assert.h>						// REMOVE LATER
# include <stdlib.h>						// for malloc etc
# include <readline/readline.h>			// for readline
# include <readline/history.h>			// for history
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <signal.h>

# define CURRENT_CHAR (*exp)->raw[(*exp)->a]
# define NEXT_CHAR (*exp)->raw[(*exp)->a + 1]
# define A (*exp)->a
# define B (*exp)->b


typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_LOGICAL,
	NODE_SUBSHELL
}	node_type;

typedef enum e_logical_op
{
	OP_AND,
	OP_OR
}	logical_op;

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

typedef struct s_cmd_node t_cmd_node;

typedef struct s_ast_node t_ast_node;

typedef struct s_pipe_data
{
	pid_t	child_pids[100];
	int	status_arr[100];
	int	pipefd[2];
	int i;
	int	num_cmds;
	int	prev_fd;
}		t_pipe_data;

typedef struct s_pipe_info
{
	t_list	*cmds;					// t_list of t_cmd_node
	int 	cmd_count;
}	t_pipe_info;

typedef struct s_logical_data
{
	logical_op	op;
	t_ast_node	*left;
	t_ast_node	*right;
}	t_logical_data;

typedef struct s_subshell_data
{
	t_ast_node	*body;
}	t_subshell_data;

typedef struct s_ast_node
{
	node_type 		type;
	t_cmd_node		*cmd;
	t_pipe_info		*pipeline;
	t_logical_data	*logical;
	t_subshell_data *subshell;
}	t_ast_node;

typedef struct s_input_node
{
	char	*name;
}			t_input_node;

typedef struct s_output_node
{
	char	*name;
	bool	create;
}			t_output_node;

typedef struct s_cmd_node
{
	char	**cmd_arr;
	t_list	*input_lst;
	t_list	*output_lst;
	int		err;
}			t_cmd_node;

typedef struct s_org_tok
{
	char	*value;
	int		oper;
	int		cmd;
	t_cmd_node	*cmd_node;
}			t_org_tok;

typedef struct s_cmd_builder
{
	t_list	*start_node;
	t_list	*end_node;
	int		num_nodes;
} 			t_cmd_builder;

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
	bool	visible;
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

typedef struct s_org_tok_mem
{
	t_list			*org_toklst;
}					t_org_tok_mem;

//------------- STRUCT PRINCPAL DE MEMÓRIA --------------

typedef struct	s_mem
{
	t_cap_mem		*capture;
	t_hd_mem		*heredoc;
	t_tok_mem		*tokenize;
	t_exp_mem		*expand;
	t_env_mem		*environs;
	t_org_tok_mem	*org_tokenize;
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
int	ft_env(t_list *envlist);
int	ft_pwd(t_list **envlist);
int	ft_echo(char **cmd_arr, t_mem **mem);
int	ft_cd(t_list **envlist, char **cmd_arr);
int	ft_export(t_list **envlist, char *variable_value);
int	ft_unset(t_list **envlist, char *variable);
int	ft_exit(char **cmd_arr, t_mem **mem);

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

//luis part 
int	ft_ast_create(t_mem **mem);

//command utils
t_cmd_builder	*create_cmd_builder(t_list **org_tok, int index_cmd);
char			**extract_cmd(t_cmd_builder *cmd_builder, int index_cmd);
void			ft_cmd_org(t_list **org_tok, t_mem **mem);
int	ft_count_items(char **str_arr);

//redirectionsft_cmd_org
int		is_redirection(char *value);
void	extract_redirections(t_list **org_tok, t_cmd_node *cmd, int index_cmd, t_mem **mem);

//ast create
t_ast_node	*parse_expression(t_list **tokens);
void		print_ast(t_ast_node *node, int depth);

//ast exec
int 		exec_ast(t_list **ms_env, t_ast_node **root, t_mem **mem);
t_env_node	*ft_init_env_node(char *variable, char *value, bool visible);
t_list		*ft_add_to_envlist(t_list **envlist, t_env_node *new_node);

//execution
int execute_pipeline(t_list **env, t_list **org_token, int num_cmds, t_mem **mem);
int	execute_command(t_list **ms_env, t_cmd_node *cur_cmd, t_mem **mem);
int	exec_single_cmd(t_list **ms_env, t_cmd_node *cur_cmd, t_mem **mem);
void	ft_ms_env_add_exit_code(t_list **envlist, char *variable, int value);

//external cmd treatment
char	**update_cmd_arr(t_list **ms_env, char **cmd_arr);

//exec external cmd
void	exec_external_cmd(t_list **ms_env, t_cmd_node *cmd);
char	**ft_ms_env_arr(t_list **ms_env);

//exec built in
bool	is_built_in(char **cmd_arr);
int	exec_built_in(t_list **ms_env, char	**cmd_arr, t_mem **mem);

//redir control
void	fd_output_redir(t_list **output_lst);
void	fd_input_redir(t_list **input_lst);
int		file_input_handler(t_list **input_lst);
int		file_output_handler(t_list **output_lst);
void	pipe_fd_control(t_pipe_data *pipe_data, t_cmd_node *cur_cmd, int pipefd[2]);

//DEBUG - REMOVER DEPOIS
void		ft_debug_list(t_list **head);

//signal
void	handle_signal_prompt(int signo);
void	handle_signal_cmd(int signo);
void	signal_before_wait(void);
void	signal_after_wait(void);
void	signal_child_process(void);

#endif