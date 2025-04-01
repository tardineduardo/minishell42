/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:36:03 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/01 12:01:02 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include "../../../include/minishell.h"

// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }					t_list;

// typedef struct s_input_node
// {
// 	t_list	*input_lst;
// 	char	*name;
// }	t_input_node;

// typedef struct s_output_node
// {
// 	t_list	*output_lst;
// 	char	*name;
// }	t_output_node;

// typedef struct 	s_new_node
// {
// 	char	*variable;
// 	char	*value;
// 	bool	readonly;
// 	bool	block_unset;
// }	t_new_node;

// typedef struct s_env_mem
// {
// 	t_list		*envlist;
// 	t_new_node	*new_node;
// 	char		**result;
// }	t_env_mem;

// typedef struct s_cmd_node
// {
// 	t_list	*cmd_lst;
// 	char	**cmd_arr;
// 	t_list	*input_lst;
// 	t_list	*output_lst;
// 	t_list	*heredoc_lst;
// 	t_list	*o_concat;
// 	int		err;
// }			t_cmd_node;

typedef struct s_ast
{
	bool			cmd;
	bool			operator;
	struct s_ast	*right;
	struct s_ast	*left;
}					t_ast;

// fork_pipe
int		*ft_pipe_control(void);
pid_t	ft_fork_control(void);

// exec_built-ins
bool	is_built_in(char **cur_arr);
void	exec_built_in(t_list **ms_env, char **cmd_arr);

// cmd_treatment
char	**update_cmd_arr(t_list **ms_env, char **cmd_arr);

//exec_external_command
void	exec_external_cmd(t_list **ms_env, t_cmd_node *cmd);

// execution
int		ft_prompt_execution(t_list **ms_env, t_list **cmd);

void	pipefd_control(int i, int num_cmds, int pipefd_0, int pipefd_1, int fd_in);
void	fd_input_redir(t_list **input_lst);
void	fd_output_redir(t_list **output_lst);

#endif