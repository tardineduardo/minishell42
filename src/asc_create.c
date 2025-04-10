/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asc_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/09 18:52:07 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    ft_ast_create(t_mem **mem)
{
	t_tok_mem	*token_mem;
	t_list      *tokens;
	t_tok_node  *token;
	char        *value;
	
	token_mem = (*mem)->tokenize;
	tokens = token_mem->toklst;
	value = ((t_tok_node *)tokens->content)->tokstr;
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
	t_list *previous_pos;
	t_list *and_or_op_pos;
	t_list *start_pos;
	start_pos = tokens;
	while (tokens)
	{
		if (ft_strcmp("&&", value) == 0
			|| ft_strcmp("||", value) == 0
			|| ft_strcmp("|", value) == 0)
		{
			and_or_pipe_op_handler(start_pos, previous_pos);
			tokens = tokens->next;
			if (tokens == NULL)
				exit(EXIT_FAILURE);
			start_pos = tokens;
		}
		else if (ft_strcmp("(", value) == 0)
			parenthesis_handler(tokens);
		previous_pos = tokens;
		tokens = tokens->next;
		value = ((t_tok_node *)tokens->content)->tokstr;
	}
}

int	parenthesis_checker(t_list *tokens)
{
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
	return (parenthesis_o, inner_parenthesis);
}

int	operator_counter(t_list * tokens)
{
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
	return (and_operator, or_operator, pipe_operator);
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