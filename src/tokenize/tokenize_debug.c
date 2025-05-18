#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/parsing.h"

void	ft_expand_toklist(t_dlist **toklst, t_mem **mem)
{
	t_dlist		*trav;
	t_tok_node	*tok_node;

	trav = *toklst;
	while (trav)
	{
		if (!trav->content)
		{
			trav = trav->next;
			continue ;
		}
		tok_node = (t_tok_node *)trav->content;
		tok_node->value = ft_expand(&tok_node->value, TOKEN, mem);
		trav = trav->next;
	}
}

void	ft_debug_list(t_dlist **head)
{
	t_dlist	*trav;

	trav = *head;
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
		int index = ((t_tok_node *)trav->content)->index;
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

		if (((t_tok_node *)trav->content)->heredoc_path)
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