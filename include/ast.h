/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/31 21:45:38 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"
# include "parsing.h"

typedef enum e_node_mode
{
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_LOGICAL,
	NODE_SUBSHELL
}	t_node_mode;

typedef enum e_logical_op
{
	OP_AND,
	OP_OR
}	t_logical_op;

typedef struct s_cmd_node	t_cmd_node;
typedef struct s_ast_node	t_ast_node;
typedef struct s_block_node	t_block_node;

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
	t_logical_op	op;
	t_ast_node		*left;
	t_ast_node		*right;
}	t_logical_data;

typedef struct s_subshell_data
{
	t_ast_node	*body;
}	t_subshell_data;

typedef struct s_ast_node
{
	t_node_mode	type;
	union
	{
		t_block_node	*block_node;
		t_pipe_info		*pipeline;
		t_logical_data	*logical;
		t_subshell_data	*subshell;
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

t_ast_node	*parse_expression(t_list **parlst, t_mem **mem);
void		ft_create_cmd_arr_and_expand(t_list **cmdlst, t_block_node **cmd,
				t_mem **mem);
void		ft_free_ast(t_ast_node **node);
int			only_redir(t_block_node *cmd, t_mem **mem);

#endif