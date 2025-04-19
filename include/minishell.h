/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/04/19 11:33:03 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"

#include <assert.h>						// REMOVE LATER
#include <stdlib.h>						// for malloc etc
#include <readline/readline.h>			// for readline
#include <readline/history.h>			// for history
# include <sys/types.h>
# include <sys/wait.h>

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

typedef struct s_ast_node t_ast_node;

typedef struct s_cmd_node t_cmd_node;

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
//---------------- PARTE LUIS ------------------------------------------

typedef struct s_ast_node
{
	node_type 		type;
	t_cmd_node		*cmd;
	t_pipe_info		*pipeline;
	t_logical_data	*logical;
	t_subshell_data *subshell;
}	t_ast_node;

typedef struct s_pipe_data
{
	int	*pipefd;
	int i;
	int	num_cmds;
	int	prev_fd;
}		t_pipe_data;

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
//------------- UM ENUM BESTA QUE EU VO TENTAR NAO USAR --------------

typedef enum e_tok_exit
{
	CONTINUE,
	END,
	ERROR,
}	t_tok_exit;

typedef enum e_quote
{
	SINGLE,
	DOUBLE,
	OFF
}	t_quote;

typedef enum e_exp_mode
{
	TOKEN,
	HEREDOC,
	HEREDOC_NORMAL,
	HEREDOC_QUOTED,	
}	t_exp_mode;



//------------- STRUCTS DOS NODES DE CADA ÁREA --------------

typedef struct 	s_hd_node
{
	char		*fpath_node;
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
	char		*delim;
	char		*fpath_cap;
}	t_hd_mem;

typedef struct s_env_mem
{
	t_list		*envlist;
	t_env_node	*new_node;
	char		**result;
}	t_env_mem;


typedef struct s_exp_mem
{
	int		i;
	char	*new;
	t_quote	quote;
}	t_exp_mem;

typedef struct s_org_tok_mem
{
	t_list			*org_toklst;
	t_cmd_builder	*cmd_builder;
	t_cmd_node		*cmd;
	char			**cmd_arr;
	t_list			*input_lst;
	t_list			*output_lst;
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
char	*ft_heredoc(t_hd_mem **hd, t_list **envlist);
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

// erros and exits
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);


//expand
char *ft_expand_string(char *string, t_exp_mode mode, t_list **envlist);

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


//DEBUG - REMOVER DEPOIS
void	ft_debug_list(t_list **head);
void 	ft_debug_list_org(t_list **head);

//LUIS org tokens
void	ft_del_org_token_node(void *content);

//LUIS cmd table
t_cmd_builder	*create_cmd_builder(t_list **org_tok, int index_cmd);
void			ft_cmd_org(t_list **org_tok);
char			**extract_cmd(t_cmd_builder *cmd_builder, int index_cmd);
void			extract_redirections(t_list **org_to, t_cmd_node *cmd, int index_cmd);
int				is_redirection(char *value);
//void			ft_del_cmd_builder_node(void *content);

//LUIS ast
//int	exec_pipe(t_list **ms_env, t_list **cmd);
int		exec_cmd(t_list **ms_env, t_cmd_node *cur_cmd);
int		ft_ast_create(t_mem **mem);
void	*ft_org_tokenize(t_mem **mem);

int		exec_pipe(t_list **ms_env, t_list **org_token, int num_cmds);
void	exec_external_cmd(t_list **ms_env, t_cmd_node *cmd);
void	exec_built_in(t_list **ms_env, char	**cmd_arr);
bool	is_built_in(char **cmd_arr);
char	**update_cmd_arr(t_list **ms_env, char **cmd_arr);
void	fd_input_redir(t_list **input_lst);
void	fd_output_redir(t_list **output_lst);
int		*ft_pipe_run(void);
pid_t	ft_fork_control(void);
void	pipe_fd_control(t_pipe_data *pipe_data, t_cmd_node *cur_cmd, int pipefd[2]);
int    counter_num_cmd(t_list **org_tok);
char	**ft_ms_env_arr(t_list **ms_env);
int		file_input_handler(t_list **input_lst);
int		file_output_handler(t_list **output_lst);
t_ast_node *parse_expression(t_list **tokens);
void 	print_ast(t_ast_node *node, int depth);