/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:42:23 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/06 15:18:05 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int    counter_num_cmd(t_list **org_tok)
{
	t_list		*cur;
	t_org_tok	*tok;
	int         total_cmds;

	cur = *org_tok;
	total_cmds = 0;
	while (cur)
	{
		tok = (t_org_tok *)cur->content;
		if (tok->cmd != -1)
			total_cmds = tok->cmd;
		cur = cur->next;
	}
	return (total_cmds + 1);
}

t_cmd_node *build_cmd_table(t_list **org_tok, int index_cmd, t_mem **mem)
{
	t_cmd_node 			*cmd;
	t_cmd_builder		*cmd_builder;

	cmd = (t_cmd_node *)malloc(sizeof(t_cmd_node));
	if (!cmd)
		return (NULL);
	cmd_builder = create_cmd_builder(org_tok, index_cmd);
	cmd->cmd_arr = extract_cmd(cmd_builder, index_cmd);
	extract_redirections(&cmd_builder->start_node, cmd, index_cmd, mem);
	return (cmd);
}

t_list	*create_new_cmd_node(t_cmd_node *cmd, int index_cmd)
{
	t_list 		*new_node;
	t_org_tok	*org_tok;

	org_tok = malloc(sizeof(t_org_tok));
	if (!org_tok)
		return (NULL);
	org_tok->value = ft_strdup("cmd table");
	org_tok->oper = -2;
	org_tok->cmd = index_cmd;
	org_tok->cmd_node = cmd;
	new_node = ft_lstnew(org_tok);
	return (new_node);
}

void	replace_with_cmd_table(t_list *new_node, t_cmd_builder *cmd_builder, t_list **org_tokens)
{
	t_list	*cur;

	new_node->next = cmd_builder->end_node->next;
	if (cmd_builder->end_node->next != NULL)
		cmd_builder->end_node->next = NULL; 
	cur = *org_tokens;
	if (cur == cmd_builder->start_node)
		*org_tokens = new_node;
	else
	{
		while (cur->next)
		{
			if (cur->next == cmd_builder->start_node)
			{
				cur->next = new_node;
				break ;
			}
			cur = cur->next;
		}
	}
}

void	ft_cmd_org(t_list **org_tok, t_mem **mem)
{
	int				total_cmds;
	int				index_cmd;
	t_list			*new_node;
	t_cmd_node 		*cmd;
	t_cmd_builder	*cmd_builder;

	total_cmds = counter_num_cmd(org_tok);
	index_cmd = 0;
	while (index_cmd < total_cmds)
	{
		cmd_builder = create_cmd_builder(org_tok, index_cmd);
		cmd = build_cmd_table(org_tok, index_cmd, mem);
		new_node = create_new_cmd_node(cmd, index_cmd);
		replace_with_cmd_table(new_node, cmd_builder, org_tok);
		//ft_clear_cmd_builder_mem(cmd_builder_mem);
		index_cmd++;
	}
}
