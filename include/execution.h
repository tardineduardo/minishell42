#ifndef EXECUTION_H
# define EXECUTION_H

# include <sys/types.h>						// for wait when executing cmds
# include <sys/wait.h>						// for wait when executing cmds
# include "minishell.h"

//redir control
void	fd_input_redir(t_list **input_lst);
void	fd_output_redir(t_list **output_lst);
void	pipe_fd_control(t_pipe_data *pipe_data, t_block_node *cur_cmd, int pipefd[2]);

//exec external cmd
void	exec_external_cmd(t_list **ms_env, t_block_node *cmd);

//exec built in
bool	is_built_in(char **cmd_arr);
int	exec_built_in(t_list **ms_env, char	**cmd_arr);

int		ft_execute(t_list **ms_env, t_ast_node **root, t_mem **mem);

#endif