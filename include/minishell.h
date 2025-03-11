/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/11 17:27:26 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>


# include <assert.h>						// REMOVE LATER					// for malloc etc
# include <readline/readline.h>			// for readline
# include <readline/history.h>			// for history

typedef struct s_env
{
	char			*variable;
	char			*value;
	bool			readonly;
	bool			block_unset;
	struct s_env	*next;
}			t_env;

typedef struct s_mem
{
	t_list		*hc_list;
	t_env		*ms_env;
	char		*hc_delim;
	char		*hc_fpath_cap;
	char		*line;
}				t_mem;


// main
char *ft_capture_line(void);
char *ft_run_command(char *line, t_mem **mem);

// heredocs
char	*ft_hc_capture(char **hc_delim, t_list **hc_list, t_mem **mem);
void	ft_hc_unlink_and_free(void *content); // needed for EXIT

// erros and exits
void	ft_init_minishell_memory(t_mem **mem, char **envp);
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);

char 	*ft_capture_command(void);
t_env	*ft_ms_env(char *envp[]);

// ms_env
void	ft_ms_env_add(t_env **ms_env, char *variable_value);
void	ft_ms_env_update(t_env **ms_env, char *variable, char *value);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
void	ft_lstdel_one_node_env(t_env **lst, char *variable);
t_env	*ft_lstnew_env(char *variable, char *value);

// built-ins
void	ft_env(t_env **ms_env);
void	ft_pwd(t_env **ms_env);
void	ft_echo(char *line, bool flag);
void	ft_cd(t_env **ms_env, char *path);
void	ft_exit(void);
void	ft_export(t_env **ms_env, char *variable_value);
void	ft_unset(t_env **ms_env, char *variable);

#endif