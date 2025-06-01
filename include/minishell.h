/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/31 21:45:38 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include <assert.h>						// REMOVE LATER
# include <stdlib.h>						// for malloc etc
# include <readline/readline.h>				// for readline
# include <readline/history.h>				// for history
# include <signal.h>						// handle signals

# include <unistd.h>
# include <sys/types.h>						// for wait when executing cmds
# include <sys/wait.h>						// for wait when executing cmds
# include <string.h>
# include <limits.h>





typedef struct s_hdc_mem t_hdc_mem;
typedef struct s_tok_mem t_tok_mem;
typedef struct s_exp_mem t_exp_mem;
typedef struct s_rdl_mem t_rdl_mem;
typedef struct s_env_mem t_env_mem;
typedef struct s_par_mem t_par_mem;
typedef struct s_ast_mem t_ast_mem;

typedef enum e_quote
{
	Q_OFF,
	Q_SINGLE,
	Q_DOUBLE,
	Q_NULL,
}	t_quote;		//compartilhado por expand e heredoc. precisa ficar aqui.

typedef enum e_mode
{
	TOKEN,
	EXPORT,
	HEREDOC,
	DELIMITER,
	INIT_MODE,
}	t_mode;			//compartilhado por expand e heredoc. precisa ficar aqui.

typedef struct	s_mem
{
	t_rdl_mem		*readline;
	t_hdc_mem		*heredoc;
	t_tok_mem		*tokenize;
	t_exp_mem		*expand;
	t_env_mem		*environs;
	t_par_mem		*parsing;
	t_ast_mem		*ast;
}	t_mem;

extern volatile int	g_signal;

void	ft_init_minishell_memory(t_mem **mem, char **envp);
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);

// handle signals
void	handle_signal_prompt(int signo);
void	handle_signal_cmd(int signo);
void	signal_before_wait(void);
void	signal_after_wait(void);
void	signal_child_process(void);

int		ft_count_items(char **str_arr);




#endif