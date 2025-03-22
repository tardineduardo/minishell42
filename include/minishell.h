/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/22 11:37:57 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"

#include <assert.h>						// REMOVE LATER
#include <stdlib.h>						// for malloc etc
#include <readline/readline.h>			// for readline
#include <readline/history.h>			// for history


typedef enum e_tok_exit
{
	SUC,
	END,
	ERR,
}	t_tok_exit;

typedef enum e_tok_type
{
	OPERATOR,
	WORD,

}	t_tok_type;


typedef struct s_env
{
	char			*variable;
	char			*value;
	bool			readonly;
	bool			block_unset;
	struct s_env	*next;
}			t_env;

typedef struct 	s_hd_node
{
	char		*fpath_node;
}	t_hd_node;

//3.criar uma struct específica
typedef struct 	s_tok_node
{
	char		*tokstr;
	t_tok_type	type;
	bool		double_quote;
	bool		single_quote;
}	t_tok_node;

//1. Criar struct para sua seçao
typedef struct	s_tok_mem
{
	t_list		*toklst;
	char		**tri_operator;
	char		**dbl_operator;
	char		*sgl_operator;
	t_list		*last_of_list;
	t_tok_node	*last_of_toks;
	t_list		*new;
	t_tok_node	*node;
	t_tok_node	*node2;
	char		*str;
}	t_tok_mem;

typedef struct	s_cap_mem
{
	char		*line;
	char		*trim;
	char		*temp;
}	t_cap_mem;

typedef struct s_hd_mem
{
	t_list		*list;
	char		*delim;
	char		*fpath_cap;
}	t_hd_mem;


typedef struct 	s_env_node
{
	char	*variable;
	char	*value;
	bool	readonly;
	bool	block_unset;
}	t_env_node;

//DAR NULL EM MEMORY NAS NOVAS!
typedef struct s_env_mem
{
	t_list		*envlist;
	t_env_node	*new_node;
	char		**result;
}	t_env_mem;


//2. Inlcuir a struct de memoria na principal
typedef struct	s_mem
{
	t_cap_mem	*capture;
	t_hd_mem	*heredoc;
	t_tok_mem	*tokenize;
	t_env_mem	*environs;
	t_env		*ms_env;
}	t_mem;

// main
char *ft_capture_line(t_cap_mem **cap);
char *ft_run_command(char *line, t_mem **mem);
//3.5 Incluir a nova funçao principal no header
void	*ft_tokenize(char *line, t_tok_mem **tok);

// heredocs
char	*ft_hc_capture(t_hd_mem **hd);
void	ft_hd_unlink_and_free(void *content); // needed for EXIT

// erros and exits
void	ft_init_minishell_memory(t_mem **mem, char **envp);
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);

//tokens
void	*ft_init_operators(t_tok_mem **tok);
void	ft_tok_free_node_in_list(void *content);

//environs
t_list	*ft_init_environs(t_env_mem **env, char **envp);
void	ft_env_node_free(void *content);

//operators
void *ft_init_operators(t_tok_mem **tok);




//-------LUIS-----//
t_env	*ft_ms_env(char *envp[]);

// built-ins
void	ft_env(t_env **ms_env);
void	ft_pwd(t_env **ms_env);
void	ft_echo(char *line, bool flag);
void	ft_cd(t_env **ms_env, char *path);
void	ft_exit(void);
void	ft_export(t_env **ms_env, char *variable_value);
void	ft_unset(t_env **ms_env, char *variable);

// ms_env
void	ft_ms_env_add(t_env **ms_env, char *variable_value);
void	ft_ms_env_update_export(t_env **ms_env, char *variable, char *value);
void	ft_ms_env_update_cd(t_env **ms_env, char *variable, char *value);
