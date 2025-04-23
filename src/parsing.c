/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_org_tok.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/19 17:02:25 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"

t_list		*ft_classify_tokens(t_mem **mem);
void		*ft_append_new_parsed_token(t_list *tokens, t_par_mem **par);
void		set_positions(t_list **org_tokens);
t_oper		update_oper2(char *value);
void		*ft_cmd_org(t_list **org_tok);


// O QUE AST RETORNA?
void	*ft_parsing(t_mem **mem) // antiga ft_ast_create()
{
	t_ast_node *root;
	t_par_mem	*par;
	t_tok_mem	*tok;

	tok = (*mem)->tokenize;
	par = (*mem)->parsing;
	par->parlst = ft_copy_tokens(tok->toklst);

	ft_cmd_org(&par->parlst);

	// AQUI: CHECAR ERROS DE SINTAXE
	if (!ft_check_syntax(tok->toklst))
	{
		//print error type like bash
		return (NULL);
	}

	root = parse_expression(&(*mem)->tokenize->toklst);
	
	//print_ast(root, 0);
	//no fim de tudo, liberar a lista de toklst
	return (mem); //trocar para reotrnar o pointer para árvore.
}

// vou copiar a toklst para: 1) simular o mesmo ambiente que a primeira versão
// 2) poder manipular e modificar parlst sem perder informações de toklst.
// depois, pode ser que eu elimine essa função aqui e manipule direto toklst.
t_list	*ft_copy_tokens(t_mem **mem)
{
	t_list		*copy;
	t_list		*trav;
	t_tok_node	*toknode;
	t_par_node	*parnode;	

	trav = (*mem)->tokenize->toklst;
	copy = malloc(sizeof(t_par_node));
	if (!copy)
		return (NULL);
	while(trav)
	{
		parnode = malloc(sizeof(t_par_node));
		if (!parnode)
			return (NULL);
		toknode = ((t_tok_node *)trav->content);
		parnode->value = ft_strdup(toknode->value);
		parnode->position = toknode->block; //padronizar depois =>block
		parnode->oper = toknode->oper;
		parnode->cmd_node = NULL;
		ft_lstadd_back(&copy, parnode);
		trav = trav->next;
	}
	return (copy);
}

void	*ft_cmd_org(t_list **parlst)
{
	int				total_cmds;
	int				index_cmd;
	t_list			*new_node;
	t_cmd_node 		*cmd;
	t_cmd_builder	*cmd_builder;

	total_cmds = counter_num_cmd(parlst);
	index_cmd = 0;
	while (index_cmd < total_cmds)
	{
		cmd_builder = create_cmd_builder(parlst, index_cmd);
		cmd = build_cmd_table(parlst, index_cmd);
		new_node = create_new_cmd_node(cmd, index_cmd);
		replace_with_cmd_table(new_node, cmd_builder, parlst);
		//ft_clear_cmd_builder_mem(cmd_builder_mem);
		index_cmd++;
	}
}

int    counter_num_cmd(t_list *parlst)
{
	t_list		*last;

	last = ft_lstlast(parlst);
	return (((t_par_node *)last->content)->position + 1);
}


t_cmd_builder	*create_cmd_builder(t_list **parlst, int index_cmd)
{
	t_cmd_builder	*cmd_builder;
	t_list			*cur;
	t_par_node		*parsnode;

	cmd_builder = malloc(sizeof(t_cmd_builder));
	if (!cmd_builder)
		return (NULL);
	cmd_builder->start_node = NULL;
	cmd_builder->end_node = NULL;
	cmd_builder->num_nodes = 0;
	cur = *parlst;
	cmd_builder->num_nodes = 0;
	while (cur)
	{
		parsnode = (t_par_node *)cur->content;
		if (parsnode->position == index_cmd)
		{
			if (cmd_builder->num_nodes == 0)
				cmd_builder->start_node = cur;
			cmd_builder->num_nodes++;
			cmd_builder->end_node = cur;
		}
		cur = cur->next;
	}
	return (cmd_builder);
}


t_syntax	ft_check_syntax(t_list *parlst)
{
	if(operator_not_supported(parlst))
		return (ERROR1);
	if (redirect_without_file(parlst))
		return (ERROR1);
	if (heredoc_without_delim(parlst))
		return (ERROR1);
	if (pipe_at_invalid_position(parlst))
		return (ERROR1);
	if (unsupported_operator(parlst))
		return (ERROR1);
	if (and_or_at_invalid_positions(parlst))
		return (ERROR1);
	if (empty_parentheses(parlst))
		return (ERROR1);
	return(SUCCESS);
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
	ft_free_and_null((void *)&(*par)->parlst);
	free(*par);
	return ;
}