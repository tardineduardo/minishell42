/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_error_and.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 17:04:55 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/27 15:31:26 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* This function only exists to comply with 42's 25-line-per-function limit. */
void	ft_print_error_and_exit(char *message, int exit_number)
{
	ft_putstr_fd(message, STDERR_FILENO);
	exit(exit_number);
}

/* This function only exists to comply with 42's 25-line-per-function limit. */
void	ft_error_free_and_exit(void *p, char *message, int exit_number)
{
	ft_putstr_fd(message, STDERR_FILENO);
	free(p);
	exit(exit_number);
}

/* This function only exists to comply with 42's 25-line-per-function limit. */
bool	ft_print_error_and_false(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (false);
}

/* This function only exists to comply with 42's 25-line-per-function limit. */
bool	ft_print_error_and_true(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (true);
}

/* This function only exists to comply with 42's 25-line-per-function limit. */
void	*ft_print_error_and_null(char *message)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (NULL);
}
