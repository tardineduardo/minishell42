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

typedef struct s_mem
{
	t_list		*hc_list;
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
void	ft_init_minishell_memory(t_mem **mem);
void	ft_clean_mem_loop(t_mem **mem);
void	ft_clear_mem_and_exit(t_mem **mem);
