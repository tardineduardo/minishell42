/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:45:01 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/14 14:52:22 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/expand.h"
#include "../include/parsing.h"
#include "../include/environs.h"
#include "../include/readline.h"
#include "../include/builtins.h"
#include "../include/execution.h"

char	**ft_expand_cmd_arr(char **cmd_arr, t_mem **mem)
{
	char	**final_cmd_arr;
	int		size_arr;
	int		i;

	i = 0;
	size_arr = ft_count_items(cmd_arr);
	final_cmd_arr = malloc(sizeof(char *) * (size_arr));
	if (!final_cmd_arr)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (cmd_arr[i] != NULL)
	{
		final_cmd_arr[i] = ft_strdup(ft_expand(&cmd_arr[i], TOKEN, mem));
		if (!final_cmd_arr[i])
		{
			perror("ft_strdup");
			while (--i >= 0)
				free(final_cmd_arr[i]);
			free(final_cmd_arr);
			exit(EXIT_FAILURE);
		}
		free(cmd_arr[i]);
		i++;
	}
	final_cmd_arr[i] = NULL;
	return (final_cmd_arr);
}
