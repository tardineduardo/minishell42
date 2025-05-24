/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/19 12:43:10 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/checks.h"

void	*fill_bnode_redir(t_dlist **toklst, t_par_node **parnd, t_par_mem **par)
{
	t_list			*redirlse;
	t_list			*copy;
	t_tok_node		*toknode;
	t_redirs_node	*redirnode;
	t_oper			oper;

	if (!(*parnd)->block_node)
		if (!init_bnd(parnd, toklst, par))
			return (NULL);
	redirnode = malloc(sizeof(t_redirs_node));
	if (!redirnode)
		return (NULL);
	oper = ((t_tok_node *)(*toklst)->content)->oper;
	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	toknode = (t_tok_node *)(*toklst)->content;
	redirnode->type = oper;
	redirnode->create = true;
	if (oper == IN_R || oper == OUT_R || oper == APPD_R)
		redirnode->name = ft_strdup(toknode->value); //RETORNAR ERRO
	if (oper == HDC_R)
		redirnode->name = ft_strdup(toknode->heredoc_path); //RETORNAR ERRO
	if (oper == APPD_R)
		redirnode->create = false;
	redirlse = ft_lstnew(redirnode);
	ft_lstadd_back(&(*parnd)->block_node->redirs_lst, redirlse);
	copy = ft_lstnew(redirnode);
	if (oper == IN_R || oper == HDC_R)
		ft_lstadd_back(&(*parnd)->block_node->input_lst, copy);
	else if (oper == OUT_R || oper == APPD_R)
		ft_lstadd_back(&(*parnd)->block_node->output_lst, copy);
	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	return (*parnd);
}

void	*fill_bnode_cmdsa(t_dlist **toklst, t_par_node **pnode, t_par_mem **par)
{
	t_tok_node	*toknode;
	char		**arraytrav;
	int			a;

	if (!(*pnode)->block_node)
		if (!init_bnd(pnode, toklst, par))
			return (NULL);
	arraytrav = (*pnode)->block_node->cmd_arr;
	toknode = (t_tok_node *)(*toklst)->content;
	a = 0;
	while (arraytrav[a])
		a++;
	arraytrav[a] = ft_strdup(toknode->value);
	if (!arraytrav[a])
		return (ft_par_syscall_error(par, "fill_blcknode_cmdarray"));
	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	return (*pnode);
}

t_block_node	*init_bnd(t_par_node **pn, t_dlist **tkls, t_par_mem **par)
{
	int	nb_tkns;

	nb_tkns = ft_dlstsize(*tkls);
	(*pn)->block_node = malloc(sizeof(t_block_node));
	if (!(*pn)->block_node)
		return (ft_par_syscall_error(par, "intit_block_node"));
	(*pn)->block_node->cmd_arr = ft_calloc(2 * nb_tkns + 1, sizeof(char *));
	if (!(*pn)->block_node->cmd_arr)
		return (ft_par_syscall_error(par, "intit_block_node"));
	(*pn)->block_node->input_lst = NULL;
	(*pn)->block_node->output_lst = NULL;
	(*pn)->block_node->redirs_lst = NULL;
	return ((*pn)->block_node);
}
