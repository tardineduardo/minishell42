/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/11 17:52:37 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void    ft_ast_create(t_mem **mem)
// {
// 	t_tok_mem	*token_mem;
// 	t_list      *tokens;
// 	t_tok_node  *token;
// 	char        *value;
	
// 	token_mem = (*mem)->tokenize;
// 	tokens = token_mem->toklst;
// 	value = ((t_tok_node *)tokens->content)->tokstr;
	/*
		I need to find the operators and organize them in a way
		that I preserve the operators precedence

		so: 
		if find parenthesis save everetyihing inside to a node
		else if pipe, create a type PIPE
		else if AND operator or OR operator, understand the delimiters where it ends 
		and where it starts
	*/

	/*
		while
			1. mark start node and end node including redirections from a command
			2. save all command infos in one char **cmd
				1. how many node we have? cmd_size
				2. malloc of a char **

				1. ft_strdup of the item from the node in each
				part of "space" of char ** created previously
	*/
// 	t_list *previous_pos;
// 	t_list *and_or_op_pos;
// 	t_list *start_pos;
// 	start_pos = tokens;
// 	while (tokens)
// 	{
// 		if (ft_strcmp("&&", value) == 0
// 			|| ft_strcmp("||", value) == 0
// 			|| ft_strcmp("|", value) == 0)
// 		{
// 			and_or_pipe_op_handler(start_pos, previous_pos);
// 			tokens = tokens->next;
// 			if (tokens == NULL)
// 				exit(EXIT_FAILURE);
// 			start_pos = tokens;
// 		}
// 		else if (ft_strcmp("(", value) == 0)
// 			parenthesis_handler(tokens);
// 		previous_pos = tokens;
// 		tokens = tokens->next;
// 		value = ((t_tok_node *)tokens->content)->tokstr;
// 	}
// }

// typedef struct s_paren_info
// {
// 	int	num_pair_parenthesis;
// 	int	num_inner_parenthesis;
// }				t_paren_info;

// t_paren_info	*parenthesis_checker(t_list *tokens)
// {
// 	t_paren_info	*paren_info;
// 	int parenthesis_o;
// 	int	parenthesis_c;
// 	int inner_parenthesis;
// 	int	last_type;
// 	char *value;

// 	parenthesis_o = 0;
// 	parenthesis_c = 0;
// 	inner_parenthesis = 0;
// 	last_type = 0;
// 	value = ((t_tok_node *)tokens->content)->tokstr;
// 	while (tokens)
// 	{
// 		if (ft_strcmp("(", value) == 0)
// 		{
// 			parenthesis_o++;
// 			if (last_type == 1 && ft_strcmp("(", value) == 0)
// 				inner_parenthesis++;
// 			last_type = 1;
// 		}
// 		else if (parenthesis_o > parenthesis_c && ft_strcmp(")", value) == 0)
// 		{
// 			parenthesis_c++;
// 			last_type = 2;
// 		}
// 		else if (parenthesis_o <= parenthesis_c && ft_strcmp(")", value) == 0)
// 			exit(EXIT_FAILURE);
// 		tokens = tokens->next;
// 		value = ((t_tok_node *)tokens->content)->tokstr;
// 	}
// 	if (parenthesis_o != parenthesis_c)
// 		exit(EXIT_FAILURE);
// 	paren_info = (t_paren_info *)malloc(sizeof(t_paren_info));
// 	if (!paren_info)
// 		return (NULL);
// 	paren_info->num_inner_parenthesis = inner_parenthesis;
// 	paren_info->num_pair_parenthesis = parenthesis_o;
// 	return (paren_info);
// }

// typedef struct s_operators_info
// {
// 	int	and_operator;
// 	int	or_operator;
// 	int	pipe_operator;
// }				t_operators_info;

// t_operators_info	*operator_counter(t_list * tokens)
// {
// 	t_operators_info *operators_info;
// 	int	and_operator;
// 	int	or_operator;
// 	int	pipe_operator;
// 	char *value;

// 	and_operator = 0;
// 	or_operator = 0;
// 	pipe_operator = 0;
// 	value = ((t_tok_node *)tokens->content)->tokstr;
// 	while (tokens)
// 	{
// 		if (ft_strcmp("&&", value) == 0)
// 			and_operator++;
// 		else if (ft_strcmp("||", value) == 0)
// 			or_operator++;
// 		else if (ft_strcmp("|", value) == 0)
// 			pipe_operator++;
// 		tokens = tokens->next;
// 		value = ((t_tok_node *)tokens->content)->tokstr;
// 	}
// 	operators_info = (t_operators_info *)malloc(sizeof(t_operators_info));
// 	if (!operators_info)
// 		return (NULL);
// 	operators_info->and_operator = and_operator;
// 	operators_info->or_operator = or_operator;
// 	operators_info->pipe_operator = pipe_operator;
// 	return (operators_info);
// }
// int	*array_position_operators(t_paren_info *paren_info, t_operators_info *operators_info)
// {
// 	int	total_ops;
// 	int	*position_ops_arr;

// 	total_ops = paren_info->num_pair_parenthesis;
// 	total_ops += operators_info->and_operator;
// 	total_ops += operators_info->or_operator;
// 	total_ops += operators_info->pipe_operator;
// 	if (total_ops > 0)
// 	{
// 		position_ops_arr = (int *)malloc(total_ops * sizeof(int));
// 		if (!position_ops_arr)
// 			return (NULL);
// 		return (position_ops_arr);
// 	}
// 	return (NULL);
// }

// int *fill_arr_pos(t_list *tokens)
// {
// 	int *pos_ops_arr;
// 	int	i;
// 	int j;
// 	char	*value;
// 	t_paren_info	*paren_info;
// 	t_operators_info	*operators_info;

// 	i = 0;
// 	j = 0;
// 	paren_info = parenthesis_checker(tokens);
// 	operators_info = operator_counter(tokens);
// 	pos_ops_arr = array_position_operators(paren_info, operators_info);
// 	while (tokens)
// 	{
// 		value = ((t_tok_node *)tokens->content)->tokstr;
// 		if (ft_strcmp("&&", value) == 0)
// 		else if (ft_strcmp("||", value) == 0)
// 		else if (ft_strcmp("(", value) == 0)
// 		else if (ft_strcmp(")", value) == 0)
// 		else if (ft_strcmp("|", value) == 0)
// 		{
// 			pos_ops_arr[i] = j;
// 			i++;
// 		}
// 		j++;
// 		tokens = tokens->next;
// 	}
// 	return (pos_ops_arr);
// }

























// t_list	*parenthesis_handler(t_list *found_open_parentheis)
// {
// 	t_list	*start_content;
// 	t_list	*tokens;
// 	int		num_nodes;
// 	char	*value;

// 	tokens = found_open_parentheis;
// 	tokens = tokens->next;
// 	if (tokens == NULL)
// 		exit(EXIT_FAILURE);
// 	start_content = tokens;
// 	num_nodes = 1;	
// 	value = ((t_tok_node *)tokens->content)->tokstr;
// 	while (ft_strcmp(")", value) != 0 && tokens != NULL)
// 	{
// 		num_nodes++;
// 		tokens = tokens->next;
// 		value = ((t_tok_node *)tokens->content)->tokstr;
// 	}
// 	if (tokens == NULL)
// 		exit(EXIT_FAILURE);
// 	return (start_content, tokens);
// }

// void	from_inner_to_outter_most_parenthesis(t_list *tokens)
// {
// 	int	inner_most;

// 	inner_most = parenthesis_counter(tokens);
// 	while (tokens != NULL && inner_most != 1)
// 	{
// 		if (ft_strcmp("(", value) == 0)
// 			inner_most--;
// 		if (inner_most == 1)
// 		{
// 			start_pos, end_pos = parenthesis_handler(tokens);
// 			break ;
// 		}
// 		tokens = tokens->next;
// 		value = ((t_tok_node *)tokens->content)->tokstr;
// 	}

// }

	/*
			3. save all redirections
				1. which type of redirection we have? define
				2. how many of them we have? input_lst or output_lst size
				3. malloc of t_list of redirection_type_lst.
				4. ft_strdup of all of the items
		break
	*/

	/*
		created all the cmd tables and redirections necessary?

		understand the idea behind of type of nodes for our ast tree.mem
		now that we have the infos of all cmds, we could make it easy to create 
		this tree.

		1. search for ()
			1. we have any other type of operator? go to the type we have
		2. search for |
			1. we have any other type of operator? go to the type we have
		3. search for &&
			1. we have any other type of operator? go to the type we have
		4. search for ||
			1. we have any other type of operator? go to the type we have
	*/

typedef struct s_org_tok
{
	char	*value;
	int		cmd;
	int		oper;
}			t_org_tok;

int	update_oper(char *value);

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
	append = ft_lstnew(org_tok);
	if (!append)
		return ;
	ft_lstadd_back(&(*org_tok_mem)->org_toklst, append);
}

void	update_cmd_org_tok(t_list  **org_tokens);

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
		else
		{
			is_new_cmd = 1;
			cmd_num++;
		}
		curr = curr->next;
	}
}
//=================================================================================== start of the treatment

void	get_cmd_nodes(t_list **org_tokens, int index_cmd)
{
	t_list		*start_node;
	t_list		*end_node;
	t_list		*cur;
	t_org_tok	*tok;
	int			num_nodes;

	cur = *org_tokens;
	num_nodes = 0;
	while (cur)
	{
		tok = (t_org_tok *)cur->content;
		if (tok->cmd == index_cmd && num_nodes == 0)
			start_node = cur;
		else if (tok->cmd != index_cmd)
			break ;
		num_nodes++;
		end_node = cur;
		cur = cur->next;
	}
	return (start_node, end_node, num_nodes);
}

void	extract_redirections()
{
	/*
	
	*/
}
void	create_cmd_table()
{
	/*

	*/
}






















void ft_debug_list_org(t_list **head);

int	ft_ast_create(t_mem **mem)
{
	ft_org_tokenize(mem);
	ft_debug_list_org(&(*mem)->org_tokenize->org_toklst);
	return (0);
}



















void	ft_del_org_token_node(void *content)
{
	t_org_tok	*org_tok_node;

	if (!content)
		return ;

	org_tok_node = (t_org_tok *)content;

//	if (tok_node->tokstr)
	ft_free_and_null((void *)&org_tok_node->value);

	ft_free_and_null((void *)&org_tok_node);
}







































void ft_debug_list_org(t_list **head)
{
	t_list *trav;

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
			ft_printf(BRIGHT_MAGENTA "%s, %d" RESET, value, oper_num);
		}
		else
		{
			ft_printf(GREEN "%s, %d" RESET, value, cmd_num);
		}

		ft_printf(GREY "] -> " RESET);
		trav = trav->next;
	}
	ft_printf(GREY "NULL" RESET);
}