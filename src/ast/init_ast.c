/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:46:46 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/28 00:32:18 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/execution.h"
#include "../../include/parsing.h"

void	*ft_init_ast_memory(t_mem **mem)
{
	(*mem)->ast = malloc(sizeof(t_ast_mem));
	if (!(*mem)->ast)
		return (NULL);
	(*mem)->ast->root = NULL;
	return ((*mem)->ast);
}

// Parse an expression
t_ast_node	*parse_expression(t_list **parlst, t_mem **mem)
{
	t_ast_node	*root;

	root = parse_logical_or(parlst, mem);
	(*mem)->ast->root = root;
	return (root);
}
