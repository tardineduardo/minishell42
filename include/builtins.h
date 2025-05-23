#ifndef BUILTINS_H
# define BUILTINS_H

# include "./minishell.h"
# include "../libs/libft/libft.h"

// built-ins
int	ft_env(t_list *envlist);
int	ft_pwd(t_list **envlist);
int	ft_echo(char **cmd_arr);
int	ft_cd(t_list **envlist, char **cmd_arr);
int	ft_exit(char **cmd_arr);
int	ft_export(t_list **envlist, char *variable_value);
int	ft_unset(t_list **envlist, char *variable);

// built-ins helpers
void	ft_ms_env_add(t_list **envlist, char *variable_value);
void	ft_ms_env_update_export(t_list **envlist, char *variable, char *value);
void	ft_ms_env_update_cd(t_list **envlist, char *variable, char *value);

#endif