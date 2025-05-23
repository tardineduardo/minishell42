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
//#include "../include/heredoc.h"
//#include "../include/tokenize.h"
//#include "../include/expand.h"
//#include "../include/parsing.h"
//#include "../include/environs.h"
#include "../include/readline.h"

void	*ft_readline(t_mem **mem)
{
	t_rdl_mem	*rdl;

	rdl = (*mem)->readline;
	rdl->line = readline("Minishell> ");
	if (!rdl->line)
		exit(0);
	if (ft_strlen(rdl->line) == 0)
		return (NULL);
	if (!ft_tokenize(&rdl->line, mem))
		return (NULL);
	if (!ft_rdl_input_loop(mem))
		return (NULL);

	add_history(rdl->line);
	return (mem);
}

char	*ft_rdl_input_loop(t_mem **mem)
{
	t_rdl_mem	*rdl;
	t_tok_mem	*tok;

	rdl = (*mem)->readline;
	tok = (*mem)->tokenize;
	while(1)
	{
		if (ft_line_is_incomplete(rdl->line))
		{
			rdl->append = (readline("append > "));
			if (!ft_tokenize(&rdl->append, mem))
				return (NULL);
			rdl->temp = rdl->line;
			rdl->line = ft_strjoin(rdl->line, rdl->append);
			ft_free_and_null((void *)&rdl->temp);
			if (ft_line_is_incomplete(rdl->line))
				continue ;
			if (!tok->remain)
				break;
			ft_free_and_null((void *)&rdl->append);
			continue ;
		}
		ft_free_and_null((void *)&tok->remain);	
		break ;
	}
	return (rdl->line);
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

void	*ft_init_rdl_memory(t_mem **mem)
{
	(*mem)->readline = malloc(sizeof(t_rdl_mem));
	if (!(*mem)->readline)
		return (NULL);
	(*mem)->readline->line = NULL;
	(*mem)->readline->trim = NULL;
	(*mem)->readline->temp = NULL;
	(*mem)->readline->append = NULL;
	return ((*mem)->readline);
}

void	ft_clear_rdl_mem(t_rdl_mem **rdl)
{
	ft_free_and_null((void *)&(*rdl)->line);
	ft_free_and_null((void *)&(*rdl)->trim);
	ft_free_and_null((void *)&(*rdl)->temp);
	free(*rdl);
	return ;
}