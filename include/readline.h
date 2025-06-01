/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:54:54 by eduribei          #+#    #+#             */
/*   Updated: 2025/05/31 22:44:44 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# define READLINE_H

# include "minishell.h" 
# include "tokenize.h"

typedef struct s_rdl_mem
{
	char		*line;
	char		*trim;
	char		*temp;
	char		*append;
}	t_rdl_mem;

int		ft_readline(t_mem **mem);
void	*ft_init_rdl_memory(t_mem **mem);
void	ft_clear_rdl_mem(t_rdl_mem **rdl);
bool	ft_has_unclosed_quotes(char *s);

#endif
