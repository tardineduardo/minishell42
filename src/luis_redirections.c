/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:16:58 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/02 15:28:29 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_redirection(char *value)
{
	if (ft_strcmp(">>", value) == 0
	|| ft_strcmp(">", value) == 0
	|| ft_strcmp("<", value) == 0)
		return (0);
	return (1);
}

void	ft_ap_nw_in(t_cmd_node *cmd, char *name)
{
	t_list			*append;
	t_input_node	*input_node;

	input_node = malloc(sizeof(t_input_node));
	if (!input_node)
		return ;
	input_node->name = ft_strdup(name);
	append = ft_lstnew(input_node);
	if (!append)
		return ;
	ft_lstadd_back(&cmd->input_lst, append);
}

void    ft_create_input_lst(t_list *tokens,  t_cmd_node *cmd, int index_cmd)
{
	t_org_tok *tok;

	while (tokens)
	{
		tok = tokens->content;
		if (tok->cmd == index_cmd && ft_strcmp("<", tok->value) == 0)
		{
			tokens = tokens->next;
			if (!tokens)
				break;
			tok = tokens->content;
			if (tok->cmd == index_cmd)
				ft_ap_nw_in(cmd, tok->value);
		}
		tokens = tokens->next;
	}
}

void	ft_ap_nw_out(t_cmd_node *cmd, char *name, bool create)
{
	t_list			*append;
	t_output_node	*output_node;

	output_node = malloc(sizeof(t_output_node));
	if (!output_node)
		return ;
	output_node->name = ft_strdup(name);
	output_node->create = create;
	append = ft_lstnew(output_node);
	if (!append)
		return ;
	ft_lstadd_back(&cmd->output_lst, append);
}

void    ft_create_output_lst(t_list *tokens,  t_cmd_node *cmd, int index_cmd)
{
	t_org_tok       *tok;

	while (tokens)
	{
		tok = tokens->content;
		if (tok->cmd == index_cmd && ft_strcmp(">>", tok->value) == 0)
		{
			tokens = tokens->next;
			if (!tokens)
				break ;
			tok = tokens->content;
			if (tok->cmd == index_cmd)
				ft_ap_nw_out(cmd, tok->value, false);
		}
		if (tok->cmd == index_cmd && ft_strcmp(">", tok->value) == 0)
		{
			tokens = tokens->next;
			if (!tokens)
				break ;
			tok = tokens->content;
			if (tok->cmd == index_cmd)
				ft_ap_nw_out(cmd, tok->value, true);
		}
		tokens = tokens->next;
	}
}

void	extract_redirections(t_list **org_tok, t_cmd_node *cmd, int index_cmd)
{
	ft_create_input_lst(*org_tok,  cmd, index_cmd);
	ft_create_output_lst(*org_tok, cmd, index_cmd);
}
