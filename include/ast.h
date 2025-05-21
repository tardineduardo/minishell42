/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:36:44 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/20 17:27:07 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"

typedef struct s_ast_mem
{
	t_ast_node	*root;
}	t_ast_mem;

void	*ft_init_ast_memory(t_mem **mem);
void	ft_clear_ast_mem(t_ast_mem **ast);

#endif