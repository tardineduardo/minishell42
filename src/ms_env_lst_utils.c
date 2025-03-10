/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_lst_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:07:31 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/10 15:04:33 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*ft_lstnew_env(char *variable, char *value)
{
	t_env	*a;

	a = malloc(sizeof(t_env));
	if (!a)
		return (NULL);
	a->variable = ft_strdup(variable);
    a->value = ft_strdup(value);
    a->readonly = false;
	a->next = NULL;
	return (a);
}

t_env	*ft_lstlast_env(t_env *lst)
{
	t_env	*trav;

	if (lst == NULL)
		return (NULL);
	trav = lst;
	while (trav->next != NULL)
		trav = trav->next;
	return (trav);
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		temp = ft_lstlast_env(*lst);
		temp->next = new;
	}
}