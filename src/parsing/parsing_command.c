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
	t_cmd_node	*cmdnode;
	t_list		*new;
	char		**arraytrav; //apagar depois, é para cmdarray
	int			a;			 //apagar depois, é para cmdarray


	toknode = (t_tok_node *)(*toklst)->content;


	//inicializa o block_node, necessário para cmdarray e cmdlist
	if (!(*pnode)->block_node)
		if (!init_bnd(pnode, toklst, par))
			return (NULL);


	//CMDARRAY - acha o primeiro index vazio na array. APAGAR depois!
	arraytrav = (*pnode)->block_node->cmd_arr;
	a = 0;
	while (arraytrav[a])
		a++;
	arraytrav[a] = ft_strdup(toknode->value);
	if (!arraytrav[a])
		return (ft_par_syscall_error(par, "fill_blcknode_cmdarray"));


	//CMDLST - nova versao para criar cmd_lst
	cmdnode = malloc(sizeof(t_cmd_node *));
	if (!cmdnode)
		return (ft_par_syscall_error(par, "fill_blcknode_cmdarray"));
	cmdnode->cmdvalue = ft_strdup(toknode->value);
	new = ft_lstnew(cmdnode);
	if (!new)
		return (ft_par_syscall_error(par, "fill_blcknode_cmdarray"));
	ft_lstadd_back(&(*pnode)->block_node->cmd_lst, new);


	//apaga o token já extraído
	ft_dlst_quick_destroy_node(toklst, *toklst, ft_del_token_node);
	return (*pnode);
}

t_block_node	*init_bnd(t_par_node **pn, t_dlist **tkls, t_par_mem **par)
{
	int	nb_tkns; //apagar, isso é para cmdarray


	//Malloc para o block_node
	(*pn)->block_node = malloc(sizeof(t_block_node));
	if (!(*pn)->block_node)
		return (ft_par_syscall_error(par, "intit_block_node"));


	//NULL nos pointers das listas
	(*pn)->block_node->cmd_lst = NULL;
	(*pn)->block_node->input_lst = NULL;
	(*pn)->block_node->output_lst = NULL;
	(*pn)->block_node->redirs_lst = NULL;


	//apagar, isso é para cmdarray
	nb_tkns = ft_dlstsize(*tkls);
	(*pn)->block_node->cmd_arr = ft_calloc(2 * nb_tkns + 1, sizeof(char *));
	if (!(*pn)->block_node->cmd_arr)
		return (ft_par_syscall_error(par, "intit_block_node"));


	return ((*pn)->block_node);
}
