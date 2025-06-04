/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:17:22 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/04 17:17:23 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "./minishell.h"
# include "../libs/libft/libft.h"

// built-ins
int		ft_env(t_list *envlist);
int		ft_pwd(t_list **envlist);
int		ft_echo(char **cmd_arr);
int		ft_cd(t_list **envlist, char **cmd_arr);
int		ft_exit(char **cmd_arr, t_mem **mem);
int		ft_export(t_list **envlist, char *variable_value);
int		ft_unset(t_list **envlist, char *variable);

// built-ins helpers
bool	ft_is_valid_varname(char *var, char *variable_value);
int		ft_error_export(char *variable);

//needed for SHELL=minishell
void	*ft_ms_env_add(t_list **envlist, char *var, char *val);
char	*ft_get_env_value(t_list **envlist, char *env_variable);

#endif