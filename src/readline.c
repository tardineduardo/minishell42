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

static void	*ft_cap_error(char *message, t_cap_mem **cap);
char	*ft_cap_input_loop(t_cap_mem **cap, t_tok_mem **tok, t_hd_mem **hd);

char	*ft_readline(t_cap_mem **cap, t_tok_mem **tok, t_hd_mem **hd)
{
	(*cap)->line = readline(YELLOW "Minishell> " RESET);

	// AS ASPAS PRECISAM SER VALIDADAS AQUI!
	// A TOKENIZAÇÃO ATUAL NÃO ACEITA ASPAS INCOMLETAS (# ímpar) OU INTERCALADAS.
	// MAS ACEITA ASPAS SEM CONTEÚDO DENTRO.

	if (!(*cap)->line)
		return (ft_cap_error("readline error", cap));

	if (ft_strlen((*cap)->line) == 0)
		return (NULL);

	if (!ft_cap_input_loop(cap, tok, hd))
		return (ft_cap_error("line capture error", cap));


	add_history((*cap)->line);
	return ((*cap)->line);
}


void	*ft_cap_error(char *message, t_cap_mem **cap)
{
	ft_dprintf(STDERR_FILENO, "Minishell: %s\n", message);

	if ((*cap)->line)
		ft_free_and_null((void *)&(*cap)->line);
	if ((*cap)->trim)
		ft_free_and_null((void *)&(*cap)->trim);
	return (NULL);
}


void	*ft_cap_syscall_error(char *message)
{
	ft_dprintf(STDERR_FILENO, "%s: %s [%i]\n", message, strerror(errno), errno);
	return (NULL);
}



char	*ft_cap_input_loop(t_cap_mem **cap, t_tok_mem **tok, t_hd_mem **hd)
{
	assert(cap);
	assert(*cap);

	while(1)
	{
		(*cap)->trim = ft_strtrim((*cap)->line, " \t");
		if (!(*cap)->trim)
			return (ft_cap_error("strtrim error", cap));

		ft_tokenize((*cap)->trim, tok, hd);


		if ((*cap)->trim[ft_strlen((*cap)->trim) - 1] == '|')
		{
			ft_free_and_null((void *)&(*cap)->trim);
			(*cap)->temp = (*cap)->line;
			(*cap)->line = ft_strjoin((*cap)->line, (readline("> ")));			
			if (!(*cap)->line)
			{
				ft_free_and_null((void *)&(*cap)->temp);
				return (NULL);
			}
			ft_free_and_null((void *)&(*cap)->temp);
			continue ;
		}
		ft_free_and_null((void *)&(*cap)->trim);
		break ;
	}


	return ((*cap)->line);




}
