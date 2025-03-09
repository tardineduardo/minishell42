/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug_print_array_of_strings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:43 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/26 13:50:38 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_debug_print_array_of_strings(char **array, int fd)
{
	int	a;

	a = 0;
	while (array[a] != NULL)
	{
		ft_dprintf(fd, "[%i] %s\n", a, array[a]);
		a++;
	}
	ft_dprintf(fd, "\n");
}
