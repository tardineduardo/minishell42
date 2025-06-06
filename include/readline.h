/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:54:54 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/05 15:32:58 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# define READLINE_H

# include "minishell.h" 
# include "tokenize.h"

typedef struct s_rdl_mem
{
	int			count;
	char		*line;
	char		*trim;
	char		*temp;
	char		*append;
}	t_rdl_mem;

int		ft_readline(t_mem **mem);
void	*ft_init_rdl_memory(t_mem **mem);
void	ft_clear_rdl_mem(t_rdl_mem **rdl);
bool	ft_has_unclosed_quotes(char *s);
bool	ft_has_unclosed_curly(char *s);
bool	ft_has_unclosed_paren(char *s);
bool	ft_ends_with_backslash(char *s);
bool	ft_is_char_escaped_rdl(char *s, int index);
void	ft_update_quote_flag_rdl(char c, t_quote *quote, int index, char *s);

#endif
