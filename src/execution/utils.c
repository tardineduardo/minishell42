/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:45:01 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/21 12:28:12 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/expand.h"
#include "../../include/parsing.h"
#include "../../include/environs.h"
#include "../../include/readline.h"
#include "../../include/builtins.h"
#include "../../include/execution.h"

char	**ft_expand_cmd_arr(char **cmd_arr, t_mem **mem)
{
	char	**final_cmd_arr;
	char	*char_expanded;
	int		size_arr;
	int		i;
	int		j;

	i = 0;
	size_arr = ft_count_items(cmd_arr);
	final_cmd_arr = ft_calloc(sizeof(char *), (size_arr + 1));
	if (!final_cmd_arr)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	j = 0;
	while (cmd_arr[i] != NULL)
	{
		char_expanded = ft_expand(&cmd_arr[i], TOKEN, mem);
		if (ft_strcmp(char_expanded, "") != 0)
		{
			final_cmd_arr[j] = ft_strdup(char_expanded);			
			if (!final_cmd_arr[j])
			{
				perror("ft_strdup");
				while (--j >= 0)
					free(final_cmd_arr[j]);
				free(final_cmd_arr);
				exit(EXIT_FAILURE);
			}
			j++;
		}
		free(cmd_arr[i]);
		i++;
	}
	final_cmd_arr[j] = NULL;
	return (final_cmd_arr);
}
