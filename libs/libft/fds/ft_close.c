/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:43 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:35:35 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* This function only exists to comply with 42's 25-line-per-function limit. */
void	ft_close(int total, ...)
{
	va_list	args;
	int		fd;
	int		a;

	va_start(args, total);
	a = 0;
	while (a < total)
	{
		fd = va_arg(args, int);
		close(fd);
		a++;
	}
	va_end(args);
}
