/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:42:45 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/05 18:09:56 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_export(char *line, int fd)
{
	char	*args[] = {"/usr/bin/env", NULL};
	char	*new_env[] = {NULL, NULL};;
	char	*var = "MY_VAR";
	char	*value = "42";
	(void)line;
	(void)fd;
	size_t len = ft_strlen(var) + ft_strlen(value) + 2;
    new_env[0] = malloc(len);
    if (!new_env[0]) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    snprintf(new_env[0], len, "%s=%s", var, value);
    if (execve(args[0], args, new_env) == -1) {
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
}