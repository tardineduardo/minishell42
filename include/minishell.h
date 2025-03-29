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


//------------- UM ENUM BESTA QUE EU VO TENTAR NAO USAR --------------

typedef enum e_tok_exit
{
	SUC,
	END,
	ERR,
}	t_tok_exit;

typedef enum e_quote
{
	SINGLE,
	DOUBLE,
	OFF
}	t_quote;

typedef enum e_exp_mode
{
	TOKEN,
	HEREDOC,
	HEREDOC_NORMAL,
	HEREDOC_QUOTED,	
}	t_exp_mode;



//------------- STRUCTS DOS NODES DE CADA ÁREA --------------

typedef struct 	s_hd_node
{
	char		*fpath_node;
}	t_hd_node;

typedef struct 	s_tok_node
{
	char		*tokstr;
	bool		double_quote;
	bool		single_quote;
}	t_tok_node;

typedef struct 	s_env_node
{
	char	*variable;
	char	*value;
	bool	readonly;
	bool	block_unset;
}	t_env_node;


//------------- STRUCTS DA MEMÓRIA DE CADA ÁREA --------------

typedef struct	s_tok_mem
{
	t_list		*toklst;
	char		**operators;
	t_list		*last_of_list;
	t_tok_node	*last_of_toks;
	t_list		*new;
	t_tok_node	*node;
	char		*str;
	t_quote		quote;
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

typedef struct s_env_mem
{
	t_list		*envlist;
	t_env_node	*new_node;
	char		**result;
}	t_env_mem;


typedef struct s_exp_mem
{
	int		i;
	char	*new;
	t_quote	quote;
}	t_exp_mem;


//------------- STRUCT PRINCPAL DE MEMÓRIA --------------

typedef struct	s_mem
{
	t_cap_mem	*capture;
	t_hd_mem	*heredoc;
	t_tok_mem	*tokenize;
	t_exp_mem	*expand;
	t_env_mem	*environs;
}	t_mem;



// main
void	ft_init_minishell_memory(t_mem **mem, char **envp);
char	*ft_readline(t_cap_mem **cap, t_tok_mem **tok, t_hd_mem **hd);
char	*ft_execute(char *line, t_mem **mem);


// heredocs
char	*ft_heredoc(t_hd_mem **hd, t_list **envlist);
void	ft_del_heredoc_node(void *content); // needed for EXIT

//tokens
void	*ft_tokenize(char *line, t_tok_mem **tok, t_hd_mem **hd);
void	*ft_init_operators(t_tok_mem **tok);
void	ft_del_token_node(void *content);

//environs
void	*ft_init_environs(t_env_mem **env, char **envp);
void	ft_del_env_node(void *content);

//operators
void *ft_init_operators(t_tok_mem **tok);

// erros and exits
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);


//expand
char *ft_expand_string(char *string, t_exp_mode mode, t_list **envlist);



// built-ins
void	ft_env(t_list *envlist);
void	ft_pwd(t_list **envlist);
void	ft_echo(char *line, bool flag);
void	ft_cd(t_list **envlist, char *new_path);
void	ft_exit(void);
void	ft_export(t_list **envlist, char *variable_value);
void	ft_unset(t_list **envlist, char *variable);

// built-ins helpers
void	ft_ms_env_add(t_list **envlist, char *variable_value);
void	ft_ms_env_update_export(t_list **envlist, char *variable, char *value);
void	ft_ms_env_update_cd(t_list **envlist, char *variable, char *value);


//DEBUG - REMOVER DEPOIS
void ft_debug_list(t_list **head);