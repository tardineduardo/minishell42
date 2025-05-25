/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs_new.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/25 11:02:30 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/heredoc.h"
#include "../../include/tokenize.h"
#include "../../include/expand.h"
#include "../../include/parsing.h"
#include "../../include/environs.h"
#include "../../include/readline.h"
#include "../../include/execution.h"

void	*ft_init_environs(t_env_mem **env, char **envp)
{
	int		i;
	char	*new_node_value;
	char	*equal_sign;

	if (!envp)
		return (NULL);
	if (!(*envp))
		return (envp);
	while (*envp != NULL)
	{
		ft_get_var_and_value(*envp);

		equal_sign = ft_strchr(*envp, '=');
		(*env)->value = ft_strdup(++equal_sign);
		(*env)->var = ft_substr(*envp)

		(*env)->value =  + 1;
		if (!(*env)->value)
			return (ft_env_error("Envp split error", env));


		(*env)->new_node = ft_init_env_node;




		envp++;
	}
	return ((*env)->envlist);
}

void	ft_get_var_and_value(char *envp)
{



}






t_env_node	*ft_init_env_node(char *variable, char *value, bool visible)
{
	t_env_node	*new;

	new = malloc(sizeof(t_env_node));
	if (!new)
		return (ft_env_syscall_error("Init node malloc error"));
	new->variable = ft_strdup(variable);
	new->value = ft_strdup(value);
	new->visible = visible;
	return (new);
}

t_list	*ft_add_to_envlist(t_list **envlist, t_env_node *new_node)
{
	t_list	*new_envlist_node;

	new_envlist_node = ft_lstnew(new_node);
	if (!(new_envlist_node))
		return (NULL);
	ft_lstadd_back(envlist, new_envlist_node);
	return (*envlist);
}
