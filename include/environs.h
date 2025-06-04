/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/25 17:18:08 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONS_H
# define ENVIRONS_H

# include "minishell.h"
# include "environs.h"

typedef struct s_env_node
{
	char	*variable;
	char	*value;
	bool	visible;
}	t_env_node;

typedef struct s_env_mem
{
	t_list		*envlist;
	t_env_node	*new_node;
	char		**ms_env_cpy;
	char		**result;
}	t_env_mem;

void		*ft_env_syscall_error(char *message);
void		*ft_env_error(char *message, t_env_mem **env);
void		*ft_get_var_and_value(char *envp, char **var, char **val);
t_env_node	*ft_init_env_node(char *var, char *val, bool visible);
t_list		*ft_add_to_envlist(t_list **envlist, t_env_node *new_node);
void		*ft_init_env_memory(t_mem **mem);
void		*ft_init_environs(t_env_mem **env, char **envp);
void		ft_clear_env_mem(t_env_mem **env);
void		ft_del_env_node(void *content);

#endif