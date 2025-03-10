/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 15:50:58 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs/libft/libft.h"

#include <assert.h>						// REMOVE LATER
#include <stdlib.h>						// for malloc etc
#include <readline/readline.h>			// for readline
#include <readline/history.h>			// for history

typedef struct 	s_hd_node
{
	char		*fpath_node;
}	t_hd_node;

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

typedef struct	s_mem
{
	t_cap_mem	*capture;
	t_hd_mem	*heredoc;
}	t_mem;

// main
char *ft_capture_line(t_cap_mem **cap, t_mem **mem);
char *ft_run_command(t_mem **mem, char *envp[]);

// heredocs
char	*ft_hc_capture(t_hd_mem **hd, t_mem **mem);
void	ft_hd_unlink_and_free(void *content); // needed for EXIT

// erros and exits
void	ft_init_minishell_memory(t_mem **mem);
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);
