/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_and.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:42:06 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/20 13:54:07 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool	ft_free_and_false(void *ptr)
{
	free(ptr);
	return (false);
}

bool	ft_free_and_true(void *ptr)
{
	free(ptr);
	return (true);
}

void	*ft_free_and_null(void *ptr)
{
	free(ptr);
	return (NULL);
}

void	*ft_free_and_exit(void *ptr, char *msg, int errnum)
{
	free(ptr);
	ft_dprintf(STDERR_FILENO, msg);
	exit(errnum);
}
