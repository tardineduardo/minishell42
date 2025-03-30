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
void	*ft_first_tokenize(t_mem **mem);
char	*ft_cap_input_loop(t_mem **mem);

// t_cap_mem *cap;
// t_env_mem *env;
// t_tok_mem *tok;
// t_hd_mem *hd;

// cap = (*mem)->capture;
// env = (*mem)->environs;
// tok = (*mem)->tokenize;
// hd = (*mem)->heredoc;

void	*ft_readline(t_mem **mem)
{
	t_cap_mem	*cap;

	cap = (*mem)->capture;
	cap->line = readline(YELLOW "Minishell> " RESET);
	if (!cap->line)
		return (NULL);
	if (ft_strlen(cap->line) == 0)
		return (NULL);
	if (!ft_first_tokenize(mem))
		return (NULL);
	if (!ft_cap_input_loop(mem))
		return (NULL);

	add_history(cap->line);
	return (mem);
}

void	*ft_first_tokenize(t_mem **mem)
{
	t_cap_mem	*cap;
	t_tok_mem	*tok;

	cap = (*mem)->capture;
	tok = (*mem)->tokenize;

	if (tok->toklst)
		return (mem);
	if (!ft_tokenize(cap->line, mem))
		return (NULL);
	return (mem);
}

char	*ft_cap_input_loop(t_mem **mem)
{
	t_cap_mem *cap;

	cap = (*mem)->capture;

	while(1)
	{
		cap->trim = ft_strtrim(cap->line, " \t");
		if (!cap->trim)
			return (NULL);
	
		if (cap->trim[ft_strlen(cap->trim) - 1] == '|')
		{
			ft_free_and_null((void *)&cap->trim);
			cap->append = (readline("> "));
			if (!ft_tokenize(cap->append, mem))
				return (NULL);
			cap->temp = cap->line;
			cap->line = ft_strjoin(cap->line, cap->append);
			ft_free_and_null((void *)&cap->trim);
			if (!cap->line)
			{
				ft_free_and_null((void *)&cap->temp);
				return (NULL);
			}
			ft_free_and_null((void *)&cap->temp);
			ft_free_and_null((void *)&cap->append);

			continue ;
		}
		ft_free_and_null((void *)&cap->trim);
		ft_free_and_null((void *)&cap->append);

		break ;
	}
	return (cap->line);




}
// void	*ft_cap_error(char *message, t_cap_mem **cap)
// {
// 	ft_dprintf(STDERR_FILENO, "Minishell: %s\n", message);

// 	if ((*cap)->line)
// 		ft_free_and_null((void *)&(*cap)->line);
// 	if ((*cap)->trim)
// 		ft_free_and_null((void *)&(*cap)->trim);
// 	return (NULL);
// }


// void	*ft_cap_syscall_error(char *message)
// {
// 	ft_dprintf(STDERR_FILENO, "%s: %s [%i]\n", message, strerror(errno), errno);
// 	return (NULL);
// }