/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/26 18:42:36 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"

t_env_node	*ft_init_env_node(char *var, char *val, bool isvisible)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	if (!new)
		return (ft_env_syscall_error("Init node malloc error"));
	new->variable = var;
	new->value = val;
	new->visible = isvisible;
	return (new);
}

void	ft_del_env_node(void *content)
{
	t_env_node	*node;

	if (!content)
		return ;
	node = (t_env_node *)content;
	ft_free_and_null((void *)&node->variable);
	ft_free_and_null((void *)&node->value);
	ft_free_and_null((void *)&node);
}

void	*ft_init_env_memory(t_mem **mem)
{
	(*mem)->environs = malloc(sizeof(t_env_mem));
	if (!(*mem)->environs)
		return (NULL);
	(*mem)->environs->envlist = NULL;
	(*mem)->environs->new_node = NULL;
	(*mem)->environs->ms_env_cpy = NULL;
	return (mem);
}

void	ft_clear_env_mem(t_env_mem **env)
{
	ft_lstclear(&(*env)->envlist, ft_del_env_node);
	ft_free_and_null_str_array(&(*env)->ms_env_cpy);
	free(*env);
	return ;
}
