/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:28:51 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/07 13:59:30 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_cd(char *ms_env[], char *new_path, int fd)
{
	int		i;
	int		item_index;
	char	**old_path;

	i = 0;
	(void)fd;
	item_index = ft_ms_env_item_index(ms_env, "PWD");
	old_path = ft_split_char(ms_env[item_index], '=');
	ft_ms_env_update(ms_env, "PWD", new_path);
	ft_ms_env_update(ms_env, "OLDPWD", old_path[1]);
	free(old_path);
}