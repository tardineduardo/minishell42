/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/07 16:54:31 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../libs/libft/libft.h"

int		ft_ms_env_item_index(char *ms_env[], char *variable);

char 	*ft_capture_command(void);
char 	*ft_run_command(char *line, char *envp[]);
char	**ft_ms_env(char *envp[]);
char	**ft_ms_env_add(char *env[], char *new_variable_value);

void	ft_ms_env_update(char *env[], char *variable, char *new_value);
void	ft_env(char *ms_env[], int fd);
void	ft_pwd(char *ms_env[], int fd);
void	ft_echo(char *line, int fd);
void	ft_cd(char *ms_env[], char *path, int fd);
void	ft_exit(void);
void	ft_export(char *line, int fd);
//void	ft_unset(char *line, int fd);
char	**ft_free_split(char **result, int i);

#endif