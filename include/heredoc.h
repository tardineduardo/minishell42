/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 22:11:48 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 21:39:58 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "minishell.h" 
# include "tokenize.h"

typedef struct s_hdc_mem
{
	int			errnum;
	char		*delim;
	char		*filepath;
	t_mode		mode;
}	t_hdc_mem;

void	*ft_init_hdc_memory(t_mem **mem);
void	ft_clear_hdc_mem(t_hdc_mem **hd);
char	*ft_heredoc(char *delimiter, t_mem **mem);
char	*ft_create_hd_filepath(int *hd_count_int, char **filepath);

//tirar isso daqui
void	heredoc_signal(void);

#endif