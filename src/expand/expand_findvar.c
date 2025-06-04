/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_findvar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/expand.h"

static t_list	*lst_sort_strlen_find_lowest(t_list *head)
{
	t_list		*lowest;
	t_list		*current;
	t_env_node	*env_low;
	t_env_node	*env_current;

	if (!head)
		return (NULL);
	lowest = head;
	env_low = (t_env_node *)lowest->content;
	current = head->next;
	while (current)
	{
		env_current = (t_env_node *)current->content;
		if (ft_strlen(env_current->variable) < ft_strlen(env_low->variable))
		{
			lowest = current;
			env_low = (t_env_node *)lowest->content;
		}
		current = current->next;
	}
	return (lowest);
}

static void	*lst_sort_strlen(t_list **to_sort)
{
	t_list	*low;
	t_list	*sorted;

	if (!to_sort || !*to_sort)
		return (NULL);
	sorted = NULL;
	while (*to_sort)
	{
		low = lst_sort_strlen_find_lowest(*to_sort);
		ft_lst_unlink_node(to_sort, low);
		ft_lstadd_front(&sorted, low);
	}
	*to_sort = sorted;
	return (*to_sort);
}

static t_exit	ft_lstcopy_and_rsort_by_len(t_list *source, t_list **sorted)
{
	t_list	*trav;
	t_list	*new_node;

	if (!source)
		return (EMPTY_VARLIST);
	if (!sorted)
		return (ERROR);
	trav = source;
	while (trav)
	{
		new_node = ft_lstnew(trav->content);
		if (!new_node)
		{
			ft_lstclear(sorted, free);
			*sorted = NULL;
			return (ERROR);
		}
		ft_lstadd_back(sorted, new_node);
		trav = trav->next;
	}
	lst_sort_strlen(sorted);
	return (SUCCESS);
}

static t_exit	ft_getvar_return(char **value, t_env_node *node,
					t_list **sortedvars, int mode)
{
	if (mode == 1)
	{
		*value = ft_strdup(node->value);
		ft_lstclear(sortedvars, NULL);
		free(sortedvars);
		return (VAR_FOUND);
	}
	ft_lstclear(sortedvars, NULL);
	free(sortedvars);
	return (VAR_NOT_FOUND);
}

t_exit	ft_get_variable_value(char *dol, char **value, t_mem **mem)
{
	t_list		*trav;
	t_env_node	*node;
	t_list		**sortedvars;

	sortedvars = malloc(sizeof(t_list *));
	*sortedvars = (*mem)->expand->sortedvars;
	if (!dol || (dol[0] != '$' && dol[0] != '{') || dol[1] == '\0')
		return (ERROR);
	ft_lstcopy_and_rsort_by_len((*mem)->environs->envlist, sortedvars);
	if (!*sortedvars)
		return (ERROR);
	trav = *sortedvars;
	while (trav)
	{
		node = (t_env_node *)trav->content;
		if (ft_strncmp(&dol[1], node->variable, ft_strlen(node->variable)) == 0)
			break ;
		trav = trav->next;
	}
	if (trav)
		return (ft_getvar_return(&(*value), node, sortedvars, 1));
	else
		return (ft_getvar_return(&(*value), node, sortedvars, 2));
}
