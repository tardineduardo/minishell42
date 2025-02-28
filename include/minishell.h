/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/02/28 18:32:49 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../libs/libft/libft.h"

char *ft_capture_command(void);
char *ft_run_command(char *line, char *envp[]);

void	ft_env(char *envp[], int fd);
void	ft_pwd(char *envp[], int fd);
void	ft_echo(char *line, int fd);
void	ft_cd(char *path, int fd);
void	ft_exit(void);

#endif