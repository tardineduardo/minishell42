/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_and.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:42:06 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 13:02:05 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool	ft_free_and_false(void **ptr)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
	return (false);
}

bool	ft_free_and_true(void **ptr)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
	return (true);
}

void	*ft_free_and_null(void **ptr)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
	return (NULL);
}

void	*ft_free_and_exit(void **ptr, char *msg, int errnum)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
	ft_dprintf(STDERR_FILENO, msg);
	exit(errnum);
}
