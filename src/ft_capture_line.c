/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_capture_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	*ft_cap_error(char *message, t_cap_mem **cap);
static char	*ft_cap_input_loop(t_cap_mem **cap, t_mem **mem);


char *ft_capture_line(t_cap_mem **cap, t_mem **mem)
{
	(*cap)->line = readline("Minishell > ");

	if (!(*cap)->line)
		return (ft_cap_error("readline error", cap));

	if (ft_strlen((*cap)->line) == 0)
		return (NULL);

	if (!ft_cap_input_loop(cap, mem))
		return (ft_cap_error("line capture error", cap));


	add_history((*cap)->line);
	return ((*cap)->line);
}


static void	*ft_cap_error(char *message, t_cap_mem **cap)
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



char	*ft_cap_input_loop(t_cap_mem **cap, t_mem **mem)
{
	assert(cap);
	assert(*cap);

	(void)mem;

	while(1)
	{
		(*cap)->trim = ft_strtrim((*cap)->line, " \t");
		if (!(*cap)->trim)
			return (ft_cap_error("strtrim error", cap));

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
