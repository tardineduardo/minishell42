/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:14:40 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 21:05:16 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <sys/types.h>
# include "minishell.h"
# include "parsing.h"

//redir control
int			pipe_fd_control_single_cmd(t_block_node *cur_cmd, t_mem **mem);
int			pipe_fd_control_only_redir(t_block_node *cur_cmd, t_mem **mem);

int			pipe_fd_control_for_ast_node(t_pipe_data *pipe_data,
				t_ast_node *cmd_node, int pipefd[2], t_mem **mem);

int			pipe_fd_control_subshell(t_pipe_data *pipe_data,
				int pipefd[2]);
bool		restore_termios(struct termios *saved);
bool		save_termios(struct termios *saved);

//redir utils
int			redir_files_validation(t_list **redir_lst,
				t_mem **mem, bool sngl_bi);
int			file_input_handler(t_list **input_lst, t_mem **mem);
int			file_output_handler(t_list **output_lst, t_mem **mem);
void		fd_input_redir(t_list **input_lst, t_mem **mem);
void		fd_output_redir(t_list **output_lst, t_mem **mem);
int			test_input_redir(char *expanded_name, t_mem **mem, bool sngl_bi);
int			teste_output_redir(char *expanded_name, bool create, t_mem **mem,
				bool is_builtin);

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
t_ast_node	*parse_logical_or(t_list **parlst, t_mem **mem);
t_ast_node	*parse_logical_and(t_list **parlst, t_mem **mem);
t_ast_node	*parse_pipeline(t_list **parlst, t_mem **mem);
t_ast_node	*parse_command_or_group(t_list **parlst, t_mem **mem);
t_ast_node	*parse_command(t_list **parlst, t_mem **mem);

int			ft_error_handler(char *err_msg,
				char *world, int exit_code, t_mem **mem);
void		ft_handle_exec_error(const char *context);

void		ft_create_arr_and_expd(t_list **cmdlst,
				t_block_node **cmd, t_mem **mem);

int			only_redir(t_block_node *cmd, t_mem **mem);
int			ft_lstsize_env(t_list *lst);

int			signal_statuses(int index, int *status);
int			signal_statuses_subshell(int status);

int			test_directory(char *expanded_name,
				t_mem **mem, bool sngl_bi);
int			test_permission(char *expanded_name,
				t_mem **mem, bool sngl_bi);
int			process_input_redir(char *expanded,
				t_mem **mem, bool sngl_bi);
int			process_output_redir(t_redirs_node *redir,
				char *expanded, t_mem **mem, bool sngl_bi);
int			process_redirection(t_redirs_node *redir,
				t_mem **mem, bool sngl_bi);
#endif