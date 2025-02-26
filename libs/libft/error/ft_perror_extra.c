/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:45:49 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:35:06 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* This function only exists to comply with 42's 25-line-per-function limit. */
void	ft_perror_extra(char *extra_argument, char *normal_perror)
{
	ft_putstr_fd(extra_argument, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(normal_perror);
}
