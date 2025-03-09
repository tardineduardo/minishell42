/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:51:31 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 16:29:51 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_run_command(char *line, t_mem **mem)
{
	if (ft_strnstr(line, "<<", ft_strlen(line)))
	{
		(*mem)->hc_delim = ft_strtrim(line, " <");
		ft_hc_capture(&(*mem)->hc_delim, &(*mem)->hc_list, mem);
	}
	else if (ft_strcmp(line, "exit") == 0)
	{
		ft_clear_mem_and_exit(mem);
		exit(0);
	}

	return (NULL);
}

