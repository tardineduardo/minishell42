/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:45:01 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/05 14:25:59 by luide-ca         ###   ########.fr       */
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

int	ft_count_expanded_items(t_list **cmdlst, t_mem **mem)
{
	t_cmd_node	*cmd_node;
	t_list		*cur_cmd;
	char		*char_expanded;
	int			size_arr;

	size_arr = 0;
	cur_cmd = *cmdlst;
	while (cur_cmd)
	{
		cmd_node = cur_cmd->content;
		char_expanded = ft_expand(&cmd_node->cmdvalue, TOKEN, mem);
		if (ft_strcmp(char_expanded, "") != 0)
			size_arr++;
		cur_cmd = cur_cmd->next;
	}
	return (size_arr);
}

void	handle_copy_error(char **cmd_arr, int j)
{
	if (!cmd_arr[j])
	{
		perror("ft_strdup");
		while (--j >= 0)
			free(cmd_arr[j]);
		free(cmd_arr);
		exit(EXIT_FAILURE);
	}
}

char	**copy_value_to_cmd_arr(t_list **cmdlst, char **cmd_arr)
{
	t_list	*cur_cmd;
	char	*char_expanded;
	int		j;

	j = 0;
	cur_cmd = *cmdlst;
	while (cur_cmd)
	{
		char_expanded = ((t_cmd_node *)cur_cmd->content)->cmdvalue;
		if (ft_strcmp(char_expanded, "") != 0)
		{
			cmd_arr[j] = ft_strdup(char_expanded);
			if (!cmd_arr[j])
				handle_copy_error(cmd_arr, j);
			j++;
		}
		cur_cmd = cur_cmd->next;
	}
	cmd_arr[j] = NULL;
	return (cmd_arr);
}

void	ft_create_arr_and_expd(t_list **cmdlst, t_block_node **cmd, t_mem **mem)
{
	int		size_arr;

	size_arr = ft_count_expanded_items(cmdlst, mem);
	if (size_arr > 0)
	{
		(*cmd)->cmd_arr = malloc(sizeof(char *) * (size_arr + 1));
		if (!(*cmd)->cmd_arr)
			ft_handle_exec_error("malloc");
		(*cmd)->cmd_arr = copy_value_to_cmd_arr(cmdlst, (*cmd)->cmd_arr);
	}
	return ;
}

int	ft_lstsize_env(t_list *lst)
{
	int			counter;
	t_list		*trav;
	t_env_node	*env_node;

	trav = lst;
	counter = 0;
	while (trav != NULL)
	{
		env_node = trav->content;
		if (env_node->visible == true)
			counter++;
		trav = trav->next;
	}
	return (counter);
}
