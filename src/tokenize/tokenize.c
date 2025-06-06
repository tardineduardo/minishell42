/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/06 19:59:00 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"


void ft_debug_list(t_dlist **head);
void ft_debug_indexes(t_dlist **head);
void	ft_print_oper(t_oper oper);


static t_tok_node	*ft_init_tknd(char *str, t_tok_node *node, t_tok_mem **tok)
{
	node->value = ft_strdup(str);
	if (!node->value)
		return (NULL);
	node->oper = ft_get_oper(str);
	node->heredoc_path = NULL;
	ft_index_for_parsing(str, node, tok);
	ft_free_and_null((void *)&str);
	return (node);
}

static t_tok_exit	ft_append_tknde(char **rem, t_tok_mem **tok, int tklimit)
{
	t_tok_node	*toknode;
	t_dlist		*append;
	char		*new_str;
	char		*temp;

	new_str = ft_substr((*rem), 0, tklimit);
	toknode = malloc(sizeof(t_tok_node));
	if (!toknode)
		return (TOK_ERROR);
	if (!ft_init_tknd(new_str, toknode, tok))
		return (TOK_ERROR);
	append = ft_dlstnew(toknode);
	if (!append)
		return (TOK_ERROR);
	ft_dlstadd_back(&(*tok)->toklst, append);
	temp = *rem;
	*rem = ft_strdup(&(*rem)[tklimit]);
	ft_free_and_null((void *)&temp);
	ft_strtrim_overwrite(rem, "\t ");
	if (!(*rem)[0])
		return (TOK_END);
	return (TOK_CONTINUE);
}

static t_tok_exit	ft_tokenize_remain(char **remain, t_tok_mem **tok)
{
	int			token_limit;
	t_tok_exit	detach_exit;

	token_limit = ft_find_token_limit((*remain), tok);
	detach_exit = ft_append_tknde(remain, tok, token_limit);
	if (detach_exit == TOK_ERROR)
		return (TOK_ERROR);
	if (detach_exit == TOK_END)
		return (TOK_END);
	return (TOK_CONTINUE);
}

static int	ft_capture_heredocs(t_tok_mem **tok, t_mem **mem)
{
	t_dlist		*trav;
	t_tok_node	*node;

	trav = (*tok)->toklst;
	while (trav)
	{
		node = (t_tok_node *)trav->content;
		if (!ft_process_heredoc(node, tok, mem))
			return ((*tok)->errnmb);
		trav = trav->next;
	}
	return (0);
}

int	ft_tokenize(char **line, t_mem **mem)
{
	t_tok_mem	*tok;
	t_tok_exit	exit;

	tok = (*mem)->tokenize;
	tok->remain = ft_strdup(*line);
	while (1)
	{
		exit = ft_tokenize_remain(&tok->remain, &tok);
		if (exit == TOK_ERROR)
			return (1);
		if (exit == TOK_END)
			break ;
	}
	ft_free_and_null((void *)&tok->remain);
	if (ft_check_syntax(tok->toklst, &tok) != 0)
		return (tok->errnmb);
	if (ft_capture_heredocs(&tok, mem) != 0)
		return (tok->errnmb);
	
	//ANTES DE WILDCARD
	ft_debug_list(&tok->toklst);	
	ft_printf("\n");
	
	if (ft_wildcards(&tok->toklst, &tok) != 0)
		return (tok->errnmb);

	//DPOIS DE WILDCARD
	ft_debug_list(&tok->toklst);	
	return (0);
}
















void ft_debug_list(t_dlist **head)
{
	t_dlist *trav;

	trav = *head;
	//ft_printf(GREY "HEAD -> " RESET);

	if (!trav)
	{
		ft_printf(GREY "NULL\n" RESET);
		return ;
	}

	while (trav)
	{
		char *value = ((t_tok_node *)trav->content)->value;

		ft_printf(GREY "[" RESET);

		if (ft_strcmp("<<<", value) == 0
			|| ft_strcmp(">>", value) == 0
			|| ft_strcmp("<<", value) == 0
			|| ft_strcmp("2>", value) == 0
			|| ft_strcmp("&>", value) == 0
			|| ft_strcmp(">", value) == 0
			|| ft_strcmp("<", value) == 0)
		{
			ft_printf(BRIGHT_BLUE "%s" RESET, value);
		}
		else if (ft_strcmp("|", value) == 0)
		{
			ft_printf(YELLOW "%s" RESET, value);
		}
		else if (ft_strcmp("&", value) == 0
			|| ft_strcmp("*", value) == 0
			|| ft_strcmp("&&", value) == 0
			|| ft_strcmp("||", value) == 0
			|| ft_strcmp("(", value) == 0
			|| ft_strcmp(")", value) == 0)
		{
			ft_printf(BRIGHT_MAGENTA "%s" RESET, value);
		}
		else
		{
			ft_printf(GREEN "%s" RESET, value);
		}

		ft_printf(GREY "] " RESET);
		trav = trav->next;
	}
	//ft_printf(GREY "NULL" RESET);

}



void ft_debug_indexes(t_dlist **head)
{
	t_dlist *trav;

	trav = *head;
	ft_printf("\n");


	ft_printf("TOKEN\t\tPOS\tGROUP\tTYPE\n");
	if (!trav)
	{
		ft_printf(GREY "NULL\n" RESET);
		return ;
	}

	while (trav)
	{
		t_oper nextoper;

		char *value = ((t_tok_node *)trav->content)->value;
		int block =  ((t_tok_node *)trav->content)->block_index;
		t_oper oper = ((t_tok_node *)trav->content)->oper;
		if (trav->next)
			nextoper = ((t_tok_node *)trav->next->content)->oper;
		else
			nextoper = WORD;

	if (oper == IN_R 	|| oper == OUT_R	|| oper == APPD_R	|| HDC_R == IN_R ||
		oper == HSTR_R	|| oper == WILD_R	|| oper == ERROR_R	|| oper == OERR_R)
			ft_printf(BRIGHT_BLUE "%s" RESET, value);
		
		else if (oper == PIPE_O)
			ft_printf(YELLOW "%s" RESET, value);
		else if (oper == AND_O || oper == OR_O || oper == GSTART_O ||
				oper == GEND_O || oper == BCKG_O)
			ft_printf(BRIGHT_MAGENTA "%s" RESET, value);
		else
			ft_printf(GREEN "%s" RESET, value);

		ft_printf(GREY " " RESET);

		ft_printf("\t\t");

		ft_printf(RED "%i " RESET, index);
		ft_printf("\t");
		ft_printf("%i ", block);
		ft_printf("\t");		
		ft_print_oper(oper);

		if(((t_tok_node *)trav->content)->heredoc_path)
		{
			ft_printf("\t");		
			ft_printf("%s", ((t_tok_node *)trav->content)->heredoc_path);
		}

		ft_printf("\n");
		(void)nextoper;
		trav = trav->next;
	}



}



void	ft_print_oper(t_oper oper)
{
	if (oper == AND_O)
		ft_printf(BRIGHT_MAGENTA "AND_O" RESET);
	else if (oper == OR_O)
		ft_printf(BRIGHT_MAGENTA "OR_O" RESET);
	else if (oper == GSTART_O)
		ft_printf(BRIGHT_MAGENTA "GSTART_O" RESET);
	else if (oper == GEND_O)
		ft_printf(BRIGHT_MAGENTA "GEND_O" RESET);
	else if (oper == PIPE_O)
		ft_printf(YELLOW "PIPE_O" RESET);
	else if (oper == BCKG_O)
		ft_printf(BRIGHT_CYAN "BCKG_O" RESET);
	else if (oper == IN_R)
		ft_printf(BRIGHT_BLUE "IN_R" RESET);
	else if (oper == OUT_R)
		ft_printf(BRIGHT_BLUE "OUT_R" RESET);
	else if (oper == APPD_R)
		ft_printf(BRIGHT_BLUE "APPD_R" RESET);
	else if (oper == ERROR_R)
		ft_printf(BRIGHT_CYAN "ERROR_R" RESET);
	else if (oper == HDC_R)
		ft_printf(BRIGHT_BLUE "HDC_R" RESET);
	else if (oper == HSTR_R)
		ft_printf(BRIGHT_CYAN "HSTR_R" RESET);
	else if (oper == WILD_R)
		ft_printf(BRIGHT_MAGENTA "WILD_R" RESET);
	else if (oper == OERR_R)
		ft_printf(BRIGHT_CYAN "OERR_R" RESET);
	else if (oper == WORD)
		ft_printf(GREEN "WORD" RESET);
	else
		ft_printf("UNKNOWN_OPERATOR (%d)", oper);
}