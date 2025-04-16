/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_org_tok.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/16 12:24:29 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	update_oper(char *value)
{
	if (ft_strcmp("&&", value) == 0)
		return (0);
	else if (ft_strcmp("||", value) == 0)
		return (1);
	else if (ft_strcmp("(", value) == 0)
		return (2);
	else if (ft_strcmp(")", value) == 0)
		return (3);
	else if (ft_strcmp("|", value) == 0)
		return (4);
	return (-1);
}

void	ft_append_new_org_token(t_list *tokens, t_org_tok_mem **org_tok_mem)
{
	t_org_tok	*org_tok;
	t_list		*append;
	char		*value;
	
	value = ((t_tok_node *)tokens->content)->tokstr;
	org_tok = malloc(sizeof(t_org_tok));
	if (!org_tok)
		return ;
	org_tok->value = ft_strdup(value);
	org_tok->oper = update_oper(value);
	org_tok->cmd = -1;
	org_tok->cmd_node = NULL;
	append = ft_lstnew(org_tok);
	if (!append)
		return ;
	ft_lstadd_back(&(*org_tok_mem)->org_toklst, append);
}

void	update_cmd_org_tok(t_list **org_tokens)
{
	int			cmd_num;
	int			is_new_cmd;
	t_list		*curr;
	t_org_tok	*tok;

	is_new_cmd = 1;
	cmd_num = 0;
	curr = *org_tokens;
	while (curr)
	{
		tok = (t_org_tok *)curr->content;
		if (tok->oper == -1)
		{
			if (is_new_cmd)
			{
				tok->cmd = cmd_num;
				is_new_cmd = 0;
			}
			else
				tok->cmd = cmd_num;
		}
		else if (tok->oper == 0 || tok->oper == 1 || tok->oper == 4)
		{
			is_new_cmd = 1;
			cmd_num++;
		}
		else
			is_new_cmd = 1;
		curr = curr->next;
	}
}

void	*ft_org_tokenize(t_mem **mem)
{
	t_org_tok_mem	*org_tok_mem;
	t_tok_mem	*token_mem;
	t_list      *tokens;
	
	token_mem = (*mem)->tokenize;
	tokens = token_mem->toklst;
	org_tok_mem = (*mem)->org_tokenize;
	while (tokens)
	{
		ft_append_new_org_token(tokens, &org_tok_mem);
		tokens = tokens->next;
	}
	update_cmd_org_tok(&org_tok_mem->org_toklst);
	return (mem);
}

// void	update_paren_org_tok(t_list **org_tokens)
// {
//     int paren_num; 
//     int paren_stack[100];
//     int stack_top;
// 	t_org_tok *tok;
// 	t_list	*cur;

// 	paren_num = 0;
// 	stack_top = -1;
//     cur = *org_tokens;
//     while (cur) {
//         tok = (t_org_tok *)cur->content;

//         if (tok->oper == 2)
// 		{
//             tok->paren = ++paren_num;
//             tok->inner_paren = (stack_top >= 0) ? paren_stack[stack_top] : -1;
//             stack_top++;
//             paren_stack[stack_top] = paren_num;
//         }
//         else if (tok->oper == 3)
// 		{
//             if (stack_top >= 0)
// 			{
//                 tok->paren = paren_stack[stack_top];
//                 tok->inner_paren = (stack_top > 0) ? paren_stack[stack_top - 1] : -1;
//                 stack_top--;
//                 paren_num--;
//             }
// 			else
// 			{
//                 tok->paren = -1;
//                 tok->inner_paren = -1;
//             }
//         }
//         else
// 		{
//             tok->paren = paren_num;
//             tok->inner_paren = (stack_top >= 0) ? paren_stack[stack_top] : -1;
//         }
//         cur = cur->next;
//     }
// }























void ft_debug_list_org(t_list **head);

int	ft_ast_create(t_mem **mem)
{
	ft_org_tokenize(mem);
	ft_cmd_org(&(*mem)->org_tokenize->org_toklst);
	ft_debug_list_org(&(*mem)->org_tokenize->org_toklst);
	return (0);
}











void	ft_del_cmd_builder_node(void *content)
{
	t_cmd_builder	*cmd_builder;

	if (!content)
		return ;

	cmd_builder = (t_cmd_builder *)content;

	if (cmd_builder->start_node)
		ft_free_and_null((void *)&cmd_builder->start_node);
	if (cmd_builder->end_node)
		ft_free_and_null((void *)&cmd_builder->end_node);
	ft_free_and_null((void *)&cmd_builder);
}

void	ft_del_org_token_node(void *content)
{
	t_org_tok	*org_tok_node;

	if (!content)
		return ;

	org_tok_node = (t_org_tok *)content;

	if (org_tok_node->value)
		ft_free_and_null((void *)&org_tok_node->value);

	ft_free_and_null((void *)&org_tok_node);
}







































void ft_debug_list_org(t_list **head)
{
	t_list *trav;
	//char *cmd_arr;

	trav = *head;
	ft_printf(GREY "HEAD -> " RESET);

	if (!trav)
	{
		ft_printf(GREY "NULL\n" RESET);
		return ;
	}

	while (trav)
	{
		char *value = ((t_org_tok *)trav->content)->value;
		int cmd_num = ((t_org_tok *)trav->content)->cmd;
		int oper_num = ((t_org_tok *)trav->content)->oper;
		// if (ft_strcmp("cmd table", value) == 0)
		// 	cmd_arr = ((t_org_tok *)trav->content)->cmd_node->cmd_arr[0];

		printf("[");

		if (ft_strcmp("<<<", value) == 0
			|| ft_strcmp(">>", value) == 0
			|| ft_strcmp("<<", value) == 0
			|| ft_strcmp("2>", value) == 0
			|| ft_strcmp("&>", value) == 0
			|| ft_strcmp(">", value) == 0
			|| ft_strcmp("<", value) == 0)
		{
			ft_printf("%s", value);
		}
		else if (ft_strcmp("|", value) == 0)
		{
			printf("%s, %d", value, oper_num);
		}
		else if (ft_strcmp("&", value) == 0
			|| ft_strcmp("*", value) == 0
			|| ft_strcmp("&&", value) == 0
			|| ft_strcmp("||", value) == 0
			|| ft_strcmp("(", value) == 0
			|| ft_strcmp(")", value) == 0)
		{
			printf("%s, %d", value, oper_num);
		}
		else
		{
			printf("%s, %d", value, cmd_num);
		}

		printf(GREY "] -> " RESET);
		trav = trav->next;
	}
	printf(GREY "NULL" RESET);
}