/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/17 18:49:49 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/checks.h"

int	ft_parsing(t_mem **mem)
{
	t_par_mem	*par;
	t_tok_mem	*tok;

	tok = (*mem)->tokenize;
	par = (*mem)->parsing;
	if (!ft_check_syntax(tok->toklst, &par))
		return (par->errnmb);
	(*mem)->parsing->errnmb = 0;
	ft_create_parlst(&tok->toklst, &par->parlst, &par);
	return (par->errnmb);
}

t_list	*ft_create_parlst(t_dlist **toklst, t_list **parlst, t_par_mem **par)
{
	t_list		*new;
	t_par_node	*parnode;
	int			num_parsnodes;
	int			a;

	num_parsnodes = count_num_parsnodes(toklst);
	a = 0;
	while (a < num_parsnodes)
	{
		parnode = malloc(sizeof(t_par_node));
		if (!parnode)
			return (ft_par_syscall_error(par, "ft_create_parlst"));
		if (!init_pnd(a, &parnode, toklst, par))
			return (NULL);
		new = ft_lstnew(parnode);
		if (!new)
			return (ft_par_syscall_error(par, "ft_create_parlst"));
		ft_lstadd_back(parlst, new);
		a++;
	}
	return (*parlst);
}

// inicializa as informações de cada parnode a partir de toklst
t_par_node	*init_pnd(int a, t_par_node **pnd, t_dlist **tklst, t_par_mem **par)
{
	t_dlist		*first;
	t_tok_node	*toknode;

	first = *tklst;
	toknode = (t_tok_node *)first->content;
	(*pnd)->block_index = toknode->block_index;
	(*pnd)->block_node = NULL;
	if (is_pipe_logical_subshell(toknode))
	{
		(*pnd)->oper = toknode->oper;
		ft_dlst_quick_destroy_node(tklst, *tklst, ft_del_token_node);
		return (*pnd);
	}
	while (1)
	{
		first = *tklst;
		if (!first)
			break ;
		toknode = (t_tok_node *)first->content;
		if (toknode->block_index > a || toknode->block_index == -1)
			break ;
		if (is_redir(toknode))
		{
			if (!fill_bnode_redir(tklst, pnd, par))
				return (NULL);
		}
		else if (is_word(toknode))
		{
			if (!fill_bnode_cmdsa(tklst, pnd, par))
				return (NULL);
		}
	}
	(*pnd)->oper = CMD;
	return (*pnd);
}

void	*fill_bnode_redir(t_dlist **toklst, t_par_node **parnd, t_par_mem **par)
{
	t_list			*redirlse;
	t_list			*copy;
	t_tok_node		*toknode;
	t_redirs_node	*redirnode;
	t_oper			oper;

	if (!(*parnd)->block_node)
		if (!intit_bnd(parnd, toklst, par))
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
		if (!intit_bnd(pnode, toklst, par))
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

// inicializa o block_node só na primeira vez que vai ser usado.
t_block_node	*intit_bnd(t_par_node **pn, t_dlist **tkls, t_par_mem **par)
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

int	count_num_parsnodes(t_dlist **toklst)
{
	t_dlist		*trav;
	t_tok_node	*toknode;
	t_tok_node	*prevtok;
	int			total_parsnodes;

	trav = *toklst;
	total_parsnodes = 0;
	while (trav)
	{
		toknode = (t_tok_node *)trav->content;
		if (trav == *toklst)
			total_parsnodes++;
		else if (is_command(toknode) && !is_command(prevtok))
			total_parsnodes++;
		else if (is_pipe_logical_subshell(toknode))
			total_parsnodes++;
		prevtok = toknode;
		trav = trav->next;
	}
	return (total_parsnodes);
}

	// if (pipe_at_invalid_position(parlst))
	// 	return (ERROR1);
	// if (and_or_at_invalid_positions(parlst))
	// 	return (ERROR1);
	// if (empty_parentheses(parlst))
	// 	return (ERROR1);
bool	ft_check_syntax(t_dlist *parlst, t_par_mem **par)
{
	if (!operators_are_supported(parlst, par))
		return (false);
	if (!redirects_are_complete(parlst, par))
		return (false);
	if (!subshell_opers_are_correct(parlst, par))
		return (false);
	if (!logic_opers_are_correct(parlst, par))
		return (false);
	if (!pipe_opers_are_correct(parlst, par))
		return (false);	
		
		
	return (true);
}

void	*operators_are_supported(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
		if (tknd->oper != WORD)
		{
			if (tknd->oper != PIPE_O
				&& tknd->oper != OUT_R
				&& tknd->oper != IN_R
				&& tknd->oper != APPD_R
				&& tknd->oper != HDC_R
				&& tknd->oper != AND_O
				&& tknd->oper != OR_O
				&& tknd->oper != GSTART_O
				&& tknd->oper != GEND_O)
				return (ft_par_syntax_error(E_NO_SUPPRT, getop(tknd), par));
		}
		trav = trav->next;
	}
	return (toklst);
}

void	*redirects_are_complete(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
		if (is_redir(tknd))
		{
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (!next || next->oper != WORD)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));
		}
		trav = trav->next;
	}
	return (toklst);
}

void	*subshell_opers_are_correct(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
	
		if (tknd->oper == GSTART_O)
		{
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (!is_word(next))
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));
			if (next->oper == GSTART_O)
				return (ft_par_syntax_error(E_NO_SUBSHE, "((", par));
				
		}
		trav = trav->next;
	}
	return (toklst);
}

void	*logic_opers_are_correct(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
	
		if (tknd->oper == AND_O || tknd->oper == OR_O)
		{
			if (!trav->prev)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(tknd), par));
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (!is_word(next) && next->oper != GSTART_O)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));				
		}
		trav = trav->next;
	}
	return (toklst);
}


void	*pipe_opers_are_correct(t_dlist *toklst, t_par_mem **par)
{
	t_dlist		*trav;
	t_tok_node	*tknd;
	t_tok_node	*next;

	trav = toklst;
	while (trav)
	{
		tknd = (t_tok_node *)trav->content;
	
		if (tknd->oper == PIPE_O)
		{
			if (!trav->prev)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(tknd), par));
			if (!trav->next)
				return (ft_par_syntax_error(E_INVAL_OPS, "newline", par));
			next = (t_tok_node *)trav->next->content;
			if (next->oper == GEND_O || next->oper == AND_O || next->oper == OR_O)
				return (ft_par_syntax_error(E_INVAL_OPS, getop(next), par));				
		}
		trav = trav->next;
	}
	return (toklst);
}





bool	is_redir(t_tok_node *toknode)
{
	if (toknode->oper == IN_R || toknode->oper == OUT_R
		|| toknode->oper == HDC_R || toknode->oper == APPD_R)
		return (true);
	if (toknode->oper == WILD_R || toknode->oper == ERROR_R
		|| toknode->oper == HSTR_R || toknode->oper == OERR_R)
		return (true);
	return (false);
}

bool	is_word(t_tok_node *toknode)
{
	if (toknode->oper == WORD)
		return (true);
	return (false);
}

bool	is_command(t_tok_node *toknode)
{
	if (toknode->oper == WORD)
		return (true);
	if (toknode->oper == IN_R || toknode->oper == OUT_R
		|| toknode->oper == HDC_R || toknode->oper == APPD_R)
		return (true);
	if (toknode->oper == WILD_R || toknode->oper == ERROR_R
		|| toknode->oper == HSTR_R || toknode->oper == OERR_R)
		return (true);
	return (false);
}

bool	is_pipe_logical_subshell(t_tok_node *toknode)
{
	if (toknode->oper == PIPE_O || toknode->oper == GSTART_O
		|| toknode->oper == GEND_O || toknode->oper == AND_O
		|| toknode->oper == OR_O)
		return (true);
	return (false);
}

void	*ft_par_syntax_error(int st_err, char *str, t_par_mem **par)
{
	(*par)->errnmb = st_err;

	if (st_err == E_NO_SUPPRT)
		ft_dprintf(STDERR_FILENO, "minishell: operator not supported `%s'\n", str);
	else if (st_err == E_INVAL_OPS)
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", str);
	else if (st_err == E_NO_SUBSHE)
		ft_dprintf(STDERR_FILENO, "minishell: subshell `%s' not supported\n", str);


	return (NULL);
}

void	*ft_par_syscall_error(t_par_mem **par, char *ftname)
{
	(*par)->errnmb = errno;
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", ftname, strerror(errno));
	return (NULL);
}

void	*ft_init_par_memory(t_mem **mem)
{
	(*mem)->parsing = malloc(sizeof(t_par_mem));
	if (!(*mem)->parsing)
		return (NULL);
	(*mem)->parsing->parlst = NULL;
	return ((*mem)->parsing);
}

void	ft_clear_par_mem(t_par_mem **par)
{
	ft_lstclear(&(*par)->parlst, ft_del_par_node);
	ft_free_and_null((void *)&(*par)->parlst);
	free(*par);
	return ;
}


void	ft_del_par_node(void *content)
{
	t_par_node	*par_node;

	if (!content)
		return ;
	par_node = (t_par_node *)content;
	if (par_node->block_node)
	{
		if (par_node->block_node->output_lst)
			ft_lstclear(&par_node->block_node->output_lst, ft_del_redir_node);
		if (par_node->block_node->input_lst)
			ft_lstclear(&par_node->block_node->input_lst, ft_del_redir_node);
		if (par_node->block_node->redirs_lst)
			ft_lstclear(&par_node->block_node->redirs_lst, ft_del_redir_node);			
		if (par_node->block_node->cmd_arr)
			ft_free_str_array(par_node->block_node->cmd_arr);
	}
	ft_free_and_null((void *)&par_node);
}

void	ft_del_redir_node(void *content)
{
	t_redirs_node	*redir_node;
	if (!content)
		return ;
	redir_node = (t_redirs_node *)content;
	ft_free_and_null((void *)&redir_node->name);
	ft_free_and_null((void *)&redir_node);
}
