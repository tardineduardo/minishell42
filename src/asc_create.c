/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asc_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/10 18:38:53 by luide-ca         ###   ########.fr       */
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

typedef struct s_paren_info
{
	int	num_pair_parenthesis;
	int	num_inner_parenthesis;
}				t_paren_info;

t_paren_info	*parenthesis_checker(t_list *tokens)
{
	t_paren_info	*paren_info;
	int parenthesis_o;
	int	parenthesis_c;
	int inner_parenthesis;
	int	last_type;
	char *value;

	parenthesis_o = 0;
	parenthesis_c = 0;
	inner_parenthesis = 0;
	last_type = 0;
	value = ((t_tok_node *)tokens->content)->tokstr;
	while (tokens)
	{
		if (ft_strcmp("(", value) == 0)
		{
			parenthesis_o++;
			if (last_type == 1 && ft_strcmp("(", value) == 0)
				inner_parenthesis++;
			last_type = 1;
		}
		else if (parenthesis_o > parenthesis_c && ft_strcmp(")", value) == 0)
		{
			parenthesis_c++;
			last_type = 2;
		}
		else if (parenthesis_o <= parenthesis_c && ft_strcmp(")", value) == 0)
			exit(EXIT_FAILURE);
		tokens = tokens->next;
		value = ((t_tok_node *)tokens->content)->tokstr;
	}
	if (parenthesis_o != parenthesis_c)
		exit(EXIT_FAILURE);
	paren_info = (t_paren_info *)malloc(sizeof(t_paren_info));
	if (!paren_info)
		return (NULL);
	paren_info->num_inner_parenthesis = inner_parenthesis;
	paren_info->num_pair_parenthesis = parenthesis_o;
	return (paren_info);
}

typedef struct s_operators_info
{
	int	and_operator;
	int	or_operator;
	int	pipe_operator;
}				t_operators_info;

t_operators_info	*operator_counter(t_list * tokens)
{
	t_operators_info *operators_info;
	int	and_operator;
	int	or_operator;
	int	pipe_operator;
	char *value;

	and_operator = 0;
	or_operator = 0;
	pipe_operator = 0;
	value = ((t_tok_node *)tokens->content)->tokstr;
	while (tokens)
	{
		if (ft_strcmp("&&", value) == 0)
			and_operator++;
		else if (ft_strcmp("||", value) == 0)
			or_operator++;
		else if (ft_strcmp("|", value) == 0)
			pipe_operator++;
		tokens = tokens->next;
		value = ((t_tok_node *)tokens->content)->tokstr;
	}
	operators_info = (t_operators_info *)malloc(sizeof(t_operators_info));
	if (!operators_info)
		return (NULL);
	operators_info->and_operator = and_operator;
	operators_info->or_operator = or_operator;
	operators_info->pipe_operator = pipe_operator;
	return (operators_info);
}
int	*array_position_operators(t_paren_info *paren_info, t_operators_info *operators_info)
{
	int	total_ops;
	int	*position_ops_arr;

	total_ops = paren_info->num_pair_parenthesis;
	total_ops += operators_info->and_operator;
	total_ops += operators_info->or_operator;
	total_ops += operators_info->pipe_operator;
	if (total_ops > 0)
	{
		position_ops_arr = (int *)malloc(total_ops * sizeof(int));
		if (!position_ops_arr)
			return (NULL);
		return (position_ops_arr);
	}
	return (NULL);
}

int *fill_arr_pos(t_list *tokens)
{
	int *pos_ops_arr;
	int	i;
	int j;
	char	*value;
	t_paren_info	*paren_info;
	t_operators_info	*operators_info;

	i = 0;
	j = 0;
	paren_info = parenthesis_checker(tokens);
	operators_info = operator_counter(tokens);
	pos_ops_arr = array_position_operators(paren_info, operators_info);
	while (tokens)
	{
		value = ((t_tok_node *)tokens->content)->tokstr;
		if (ft_strcmp("&&", value) == 0)
		else if (ft_strcmp("||", value) == 0)
		else if (ft_strcmp("(", value) == 0)
		else if (ft_strcmp(")", value) == 0)
		else if (ft_strcmp("|", value) == 0)
		{
			pos_ops_arr[i] = j;
			i++;
		}
		j++;
		tokens = tokens->next;
	}
	return (pos_ops_arr);
}

























t_list	*parenthesis_handler(t_list *found_open_parentheis)
{
	t_list	*start_content;
	t_list	*tokens;
	int		num_nodes;
	char	*value;

	tokens = found_open_parentheis;
	tokens = tokens->next;
	if (tokens == NULL)
		exit(EXIT_FAILURE);
	start_content = tokens;
	num_nodes = 1;	
	value = ((t_tok_node *)tokens->content)->tokstr;
	while (ft_strcmp(")", value) != 0 && tokens != NULL)
	{
		num_nodes++;
		tokens = tokens->next;
		value = ((t_tok_node *)tokens->content)->tokstr;
	}
	if (tokens == NULL)
		exit(EXIT_FAILURE);
	return (start_content, tokens);
}

void	from_inner_to_outter_most_parenthesis(t_list *tokens)
{
	int	inner_most;

	inner_most = parenthesis_counter(tokens);
	while (tokens != NULL && inner_most != 1)
	{
		if (ft_strcmp("(", value) == 0)
			inner_most--;
		if (inner_most == 1)
		{
			start_pos, end_pos = parenthesis_handler(tokens);
			break ;
		}
		tokens = tokens->next;
		value = ((t_tok_node *)tokens->content)->tokstr;
	}

}

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


typedef struct s_ast_org
{
	char	*value;
	int		cmd;
	int		operator;
}			t_ast_org;

void	add_info(t_list *tokens)
{
	int	size_lst_tokens;
	t_list	**ast_org_lst;

	size_lst_tokens = ft_lstsize(tokens);
	ast_org_lst = (t_list )
}