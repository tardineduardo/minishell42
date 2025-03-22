/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs_NEW.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/22 11:39:46 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//errors
void	*ft_env_syscall_error(char *message);
void	*ft_env_error(char *message, t_cap_mem **cap);



//-------------------------------------------NEW
t_list	*ft_init_environs(t_env_mem **env, char **envp)
{
	assert(env);
	assert(*env);

	if (!envp || !(* envp))
		return (NULL);

	while (*envp != NULL)
	{
		(*env)->result = ft_split_char(*envp, '=');
		if (!(*env)->result)
			return (ft_env_syscall_error("Env split error"));

		if (result[1])
			new_node = ft_lstnew_env(result[0], result[1]);
		else
			new_node = ft_lstnew_env(result[0], "");
		if (!new_node)
		{
			ft_free_split(result, 2);
			return NULL;
		}
		ft_lstadd_back_env(&env_cpy, new_node);
		ft_free_split(result, 2);
		envp++;
	}
	ft_ms_env_update_bools(&env_cpy);
	return (env_cpy);


}


t_env_node	*ft_init_env_node(char *variable, char *value)
{
	t_env_node *



}




// --------------- ERRORS --------------------

void	*ft_env_syscall_error(char *message)
{
	ft_dprintf(STDERR_FILENO, "%s: %s [%i]\n", message, strerror(errno), errno);
	return (NULL);
}

void	*ft_env_error(char *message, t_env_mem **env)
{
	ft_dprintf(STDERR_FILENO, "Minishell: %s\n", message);

	// if ((*cap)->line)
	// 	ft_free_and_null((void *)&(*cap)->line);
	// etc
}