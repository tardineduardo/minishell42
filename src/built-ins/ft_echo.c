/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 17:52:39 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/02 21:47:54 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"  
#include "../../include/heredoc.h"
#include "../../include/expand.h"
#include "../../include/tokenize.h"
#include "../../include/environs.h"
#include "../../include/heredoc.h"
#include "../../include/builtins.h"

int	ft_n_combinations_of_n(char *word)
{
	int	i;

	if (!word || word[0] != '-')
		return (1);
	i = 1;
	while (word[i])
	{
		if (word[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	ft_print_echo_from(char **cmd_arr, int start, bool newline)
{
	int	j;

	j = start;
	while (cmd_arr[j])
	{
		ft_printf("%s", cmd_arr[j]);
		if (cmd_arr[j + 1] != NULL)
			ft_printf(" ");
		j++;
	}
	if (newline)
		ft_printf("\n");
}

int	ft_echo(char **cmd_arr)
{
	int		i;
	bool	flag;

	i = 1;
	flag = false;
	while (cmd_arr[i] && ft_n_combinations_of_n(cmd_arr[i]) == 0)
	{
		flag = true;
		i++;
	}
	ft_print_echo_from(cmd_arr, i, !flag);
	return (0);
}
