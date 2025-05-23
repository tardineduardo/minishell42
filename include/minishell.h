/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/21 12:43:09 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include <assert.h>						// REMOVE LATER
# include <stdlib.h>						// for malloc etc
# include <readline/readline.h>				// for readline
# include <readline/history.h>				// for history
# include <signal.h>						// handle signals

typedef struct s_hdc_mem t_hdc_mem;
typedef struct s_tok_mem t_tok_mem;
typedef struct s_exp_mem t_exp_mem;
typedef struct s_rdl_mem t_rdl_mem;
typedef struct s_env_mem t_env_mem;
typedef struct s_par_mem t_par_mem;
typedef struct s_ast_mem t_ast_mem;

typedef enum e_quote
{
	Q_OFF,
	Q_SINGLE,
	Q_DOUBLE,
	Q_NULL,
}	t_quote;		//compartilhado por expand e heredoc. tentar tirar daqui.

typedef enum e_mode
{
	TOKEN,
	EXPORT,
	HEREDOC,
	DELIMITER,
	INIT_MODE,
}	t_mode;			//compartilhado por expand e heredoc. tentar tirar daqui.

typedef struct	s_mem
{
	t_rdl_mem		*readline;
	t_hdc_mem		*heredoc;
	t_tok_mem		*tokenize;
	t_exp_mem		*expand;
	t_env_mem		*environs;
	t_par_mem		*parsing;
	t_ast_mem		*ast;
}	t_mem;

void	ft_init_minishell_memory(t_mem **mem, char **envp);
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);
char	*ft_capture_in_interactive_mode(char *prompt);

// handle signals
void	handle_signal_prompt(int signo);
void	handle_signal_cmd(int signo);
void	signal_before_wait(void);
void	signal_after_wait(void);
void	signal_child_process(void);

typedef enum e_node_mode
{
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_LOGICAL,
	NODE_SUBSHELL
}	node_mode;

typedef enum e_logical_op
{
	OP_AND,
	OP_OR
}	logical_op;

typedef struct s_cmd_node t_cmd_node;

typedef struct s_ast_node t_ast_node;

typedef struct s_pipe_data
{
	pid_t	child_pids[100];
	int		status_arr[100];
	int		pipefd[2];
	int		i;
	int		num_cmds;
	int		prev_fd;
}	t_pipe_data;

typedef struct s_pipe_info
{
	t_list	*cmds;
	int		cmd_count;
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

typedef struct s_block_node
{
	char	**cmd_arr;
	t_list	*input_lst;
	t_list	*output_lst;
	t_list	*redirs_lst;
	int		err;
}			t_block_node;

typedef struct s_ast_node
{
	node_mode 		type;
	union
	{
		t_block_node	*block_node;
		t_pipe_info		*pipeline;
		t_logical_data	*logical;
		t_subshell_data *subshell;
	};
}	t_ast_node;

typedef struct s_output_node
{
	char	*name;
	bool	create;
}			t_output_node;

typedef struct s_input_node
{
	char	*name;
}	t_input_node;

t_ast_node *parse_expression(t_list **parlst, t_mem **mem);
void	free_ast(t_ast_node *node);
#endif