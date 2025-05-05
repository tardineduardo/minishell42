/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*ft_cap_error(char *message, t_cap_mem **cap);
bool	ft_line_is_incomplete(char *s);
void	*ft_first_tokenize(t_mem **mem);
char	*ft_cap_input_loop(t_mem **mem);

void	*ft_readline(t_mem **mem)
{
	t_cap_mem	*cap;

	cap = (*mem)->capture;
	cap->line = readline(YELLOW "Minishell> " RESET);
	//the same as signal Crtl-D
	if (!cap->line)
	{
		exit(0);
		return (0);
	}
	if (ft_strlen(cap->line) == 0)
		return (NULL);
	if (!ft_tokenize(&cap->line, mem))
		return (NULL);
	if (!ft_cap_input_loop(mem))
		return (NULL);
	add_history(cap->line);
	return (mem);
}

char	*ft_cap_input_loop(t_mem **mem)
{
	t_cap_mem	*cap;
	t_tok_mem	*tok;

	cap = (*mem)->capture;
	tok = (*mem)->tokenize;
	while(1)
	{
		if (ft_line_is_incomplete(cap->line))
		{
			cap->append = (readline(YELLOW "append > " RESET));
			if (!ft_tokenize(&cap->append, mem))
				return (NULL);
			cap->temp = cap->line;
			cap->line = ft_strjoin(cap->line, cap->append);
			ft_free_and_null((void *)&cap->temp);
			if (!tok->remain)
				break;
			ft_free_and_null((void *)&cap->append);
			continue ;
		}
		ft_free_and_null((void *)&tok->remain);	
		break ;
	}
	return (cap->line);
}


bool	ft_line_is_incomplete(char *s)
{
	char	*new;
	size_t	len;

	new = ft_strtrim(s, " \t");
	if (!new)
		return (false);
	len = ft_strlen(new);
	if (len > 0 && new[len - 1] == '|')
	{
		free(new);
		return (true);
	}
	free(new);
	return (false);
}
