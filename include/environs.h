/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:10:16 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/20 14:08:21 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONS_H
# define ENVIRONS_H

# include "minishell.h"
# include "heredoc.h"
# include "expand.h"
# include "tokenize.h"
# include "environs.h"
# include "../libs/libft/libft.h"

typedef struct s_mem	t_mem;

typedef struct s_env_node
{
	char	*variable;
	char	*value;
	bool	readonly;
	bool	block_unset;
	bool	visible;
}	t_env_node;

typedef struct s_env_mem
{
	t_list		*envlist;
	t_env_node	*new_node;
	char		**ms_env_cpy;
	char		**result;
}	t_env_mem;

void		ft_env_block_unset(t_list **envlist);
void		ft_env_readonly(t_list **envlist);
void		*ft_env_syscall_error(char *message);
void		*ft_env_error(char *message, t_env_mem **env);
t_env_node	*ft_init_env_node(char *variable, char *value, bool visible);
t_list		*ft_add_to_envlist(t_list **envlist, t_env_node *new_node);
void		*ft_init_env_memory(t_mem **mem);
void		*ft_init_environs(t_env_mem **env, char **envp);
void		ft_clear_env_mem(t_env_mem **env);
void		ft_del_env_node(void *content);

#endif