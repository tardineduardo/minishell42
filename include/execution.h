/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:14:40 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/04 17:21:44 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <sys/types.h>						// for wait when executing cmds
# include <sys/wait.h>						// for wait when executing cmds
# include "minishell.h"

//redir control
int			pipe_fd_control(t_pipe_data *pipe_data,
				t_block_node *cur_cmd, int pipefd[2], t_mem **mem);
int			pipe_fd_control_single_cmd(t_block_node *cur_cmd, t_mem **mem);
int			pipe_fd_control_only_redir(t_block_node *cur_cmd, t_mem **mem);

//redir utils
int			redir_files_validation(t_list **redir_lst, t_mem **mem);
int			file_input_handler(t_list **input_lst, t_mem **mem);
int			file_output_handler(t_list **output_lst, t_mem **mem);
void		fd_input_redir(t_list **input_lst, t_mem **mem);
void		fd_output_redir(t_list **output_lst, t_mem **mem);
void		test_input_redir(char *expanded_name, t_mem **mem);
void		teste_output_redir(char *expanded_name, bool create, t_mem **mem);

//exec external cmd
void		exec_external_cmd(t_list **ms_env, t_block_node *cmd, t_mem **mem);
char		**update_cmd_arr(t_list **ms_env, char **cmd_arr, t_mem **mem);

//execution
int			execute_command(t_list **ms_env,
				t_block_node *cur_cmd, t_mem **mem);
int			print_child_statuses(t_pipe_data *p, int *status);

//exec built in
bool		is_built_in(char **cmd_arr);
int			exec_built_in(t_list **ms_env, char	**cmd_arr, t_mem **mem);

int			ft_execute(t_list **ms_env, t_ast_node **root, t_mem **mem);

//execution utils
char		**ft_expand_cmd_arr(char **cmd_arr, t_mem **mem);

//execution single cmd
int			exec_single_cmd(t_list **ms_env, t_block_node *cmd, t_mem **mem);

// execution pipe
int			exec_pipeline(t_list **env, t_list **parlst,
				int num_cmds, t_mem **mem);

//exit code
void		ft_ms_env_add_exit_code(t_list **envlist,
				char *variable, int value);
void		ft_ms_env_update_exit_code(t_list **envlist,
				char *variable, int value);

//ast utils
t_ast_node	*create_command_node(t_block_node *block_node);
t_ast_node	*create_pipeline_node(t_list *cmds_lst);
t_ast_node	*create_logical_node(t_logical_op op,
				t_ast_node *left, t_ast_node *right);
t_ast_node	*create_group_node(t_ast_node *body);

//ast parsing
t_ast_node	*parse_expression(t_list **parlst, t_mem **mem);
t_ast_node	*parse_logical_or(t_list **parlst, t_mem **mem);
t_ast_node	*parse_logical_and(t_list **parlst, t_mem **mem);
t_ast_node	*parse_pipeline(t_list **parlst, t_mem **mem);
t_ast_node	*parse_command_or_group(t_list **parlst, t_mem **mem);
t_ast_node	*parse_command(t_list **parlst, t_mem **mem);

int			ft_error_handler(char *err_msg,
				char *world, int exit_code, t_mem **mem);
void		ft_handle_exec_error(const char *context);

#endif