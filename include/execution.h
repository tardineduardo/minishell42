#ifndef EXECUTION_H
# define EXECUTION_H

# include <sys/types.h>						// for wait when executing cmds
# include <sys/wait.h>						// for wait when executing cmds
# include "minishell.h"


//redir control
int	pipe_fd_control(t_pipe_data *pipe_data, t_block_node *cur_cmd, int pipefd[2], t_mem **mem);
int	pipe_fd_control_single_cmd(t_block_node *cur_cmd, t_mem **mem);

//exec external cmd
void	exec_external_cmd(t_list **ms_env, t_block_node *cmd);
int		ft_count_items(char **str_arr);
char	**update_cmd_arr(t_list **ms_env, char **cmd_arr);

//exec built in
bool	is_built_in(char **cmd_arr);
int	exec_built_in(t_list **ms_env, char	**cmd_arr);

int		ft_execute(t_list **ms_env, t_ast_node **root, t_mem **mem);


//exit code
void	ft_ms_env_add_exit_code(t_list **envlist, char *variable, int value);
void	ft_ms_env_update_exit_code(t_list **envlist, char *variable, int value);

#endif