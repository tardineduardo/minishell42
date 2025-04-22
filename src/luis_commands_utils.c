/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_commands_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:37:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/16 13:53:15 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"

t_cmd_builder	*create_cmd_builder(t_list **org_tok, int index_cmd)
{
	t_cmd_builder	*cmd_builder;
	t_list			*cur;
	t_org_tok		*tok;

	cmd_builder = malloc(sizeof(t_cmd_builder));
	if (!cmd_builder)
		return (NULL);
	cmd_builder->start_node = NULL;
	cmd_builder->end_node = NULL;
	cmd_builder->num_nodes = 0;
	cur = *org_tok;
	cmd_builder->num_nodes = 0;
	while (cur)
	{
		tok = (t_org_tok *)cur->content;
		if (tok->cmd == index_cmd)
		{
			if (cmd_builder->num_nodes == 0)
				cmd_builder->start_node = cur;
			cmd_builder->num_nodes++;
			cmd_builder->end_node = cur;
		}
		cur = cur->next;
	}
	return (cmd_builder);
}

int	extract_num_nodes_cmd(t_cmd_builder *cmd_builder, int index_cmd)
{
	int		counter;
	t_list	*cur;

	counter = 0;
	cur = cmd_builder->start_node;
	while (cur && ((t_org_tok *)cur->content)->cmd == index_cmd)
	{
		if (is_redirection(((t_org_tok *)cur->content)->value) == 1)
			counter++;
		else
			break ;
		cur = cur->next;
	}
	return (counter);
}

char	**extract_cmd(t_cmd_builder *cmd_builder, int index_cmd)
{
	char	**cmd_arr;
	int		num_nodes;
	int		i;
	t_list	*cur;

	i = 0;
	num_nodes = extract_num_nodes_cmd(cmd_builder, index_cmd);
	cmd_arr = (char **)malloc(sizeof(char *) * (num_nodes + 1));
	if (!cmd_arr)
		return (NULL);
	cur = cmd_builder->start_node;
	while (i < num_nodes)
	{
		cmd_arr[i] = ft_strdup(((t_org_tok *)cur->content)->value);
		i++;
		cur = cur->next;
	}
	cmd_arr[i] = NULL;
	return (cmd_arr);
}





// void	ft_del_cmd_builder_node(void *content)
// {
// 	t_cmd_builder	*cmd_builder;

// 	if (!content)
// 		return ;

// 	cmd_builder = (t_cmd_builder *)content;

// 	if (cmd_builder->start_node)
// 		ft_free_and_null((void *)&cmd_builder->start_node);
// 	if (cmd_builder->end_node)
// 		ft_free_and_null((void *)&cmd_builder->end_node);
// 	ft_free_and_null((void *)&cmd_builder);
// }

// void	ft_clear_cmd_builder_mem(t_org_tok_mem **org_tok)
// {
// 	ft_lstclear(&(*org_tok)->org_toklst, ft_del_cmd_builder_node);
// 	free(*org_tok);
// 	return ;
// }


