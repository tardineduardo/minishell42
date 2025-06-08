/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:42:22 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/08 19:02:16 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/readline.h"

static void	ft_addhist(char *line)
{
	char	*temp;

	temp = ft_strtrim(line, " \t");
	if (!ft_isspace(temp[0]))
		add_history(line);
	free(temp);
	return ;
}

static bool	ft_line_is_incomplete(char *s)
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

static char	*ft_rdl_input_loop(t_mem **mem)
{
	t_rdl_mem	*rdl;

	rdl = (*mem)->readline;
	while (1)
	{
		if (ft_line_is_incomplete(rdl->line))
		{
			rdl->append = (readline(YELLOW "append > " RESET));
			if (!ft_tokenize(&rdl->append, mem))
				return (NULL);
			rdl->temp = rdl->line;
			rdl->line = ft_strjoin(rdl->line, rdl->append);
			ft_free_and_null((void *)&rdl->temp);
			if (ft_line_is_incomplete(rdl->line))
				continue ;
			if (!(*mem)->tokenize->remain)
				break ;
			ft_free_and_null((void *)&rdl->append);
			continue ;
		}
		ft_free_and_null((void *)&(*mem)->tokenize->remain);
		break ;
	}
	return (rdl->line);
}

bool	ft_syntax_is_valid(char *line)
{
	if (ft_has_unclosed_quotes(line))
		return (false);
	if (ft_has_unclosed_paren(line))
		return (false);
	if (ft_has_unclosed_curly(line))
		return (false);
	if (ft_ends_with_backslash(line))
		return (false);
	return (true);
}

int	ft_readline(t_mem **mem)
{
	t_rdl_mem	*rdl;
	static int	line_number;
	int			tokresult;

	rdl = (*mem)->readline;
	rdl->count = ++line_number;
	rdl->line = readline("Minishell> ");
	if (!rdl->line)
	{
		ft_clear_mem_and_exit(mem);
		exit(0);
	}
	if (g_signal == SIGINT)
		return (130);
	ft_addhist(rdl->line);
	if (!ft_syntax_is_valid(rdl->line))
		return (1);
	tokresult = ft_tokenize(&rdl->line, mem);
	if (tokresult != 0)
		return (tokresult);
	if (!ft_rdl_input_loop(mem))
		return (1);
	return (0);
}
