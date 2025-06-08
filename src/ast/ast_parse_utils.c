/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parse_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:53:05 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 18:02:59 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/parsing.h"

t_list	*ft_iterative_pipeline_parse(t_list **parlst,
		t_mem **mem, t_ast_node *node)
{
	t_list		*cmds_lst;
	t_ast_node	*right;
	t_list		*new_node;

	cmds_lst = ft_lstnew(node);
	if (!cmds_lst)
		return (NULL);
	while (*parlst && ((t_par_node *)(*parlst)->content)->oper == 4)
	{
		*parlst = (*parlst)->next;
		right = parse_command_or_group(parlst, mem);
		if (!right || (right->type != NODE_COMMAND
				&& right->type != NODE_SUBSHELL))
			ft_error_handler(" Invalid pipeline structure\n",
				"node", 1, mem);
		new_node = ft_lstnew(right);
		if (!new_node)
		{
			ft_lstclear(&cmds_lst, NULL);
			return (NULL);
		}
		ft_lstadd_back(&cmds_lst, new_node);
	}
	return (cmds_lst);
}
