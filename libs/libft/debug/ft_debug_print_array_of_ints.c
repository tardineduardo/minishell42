/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug_print_array_of_ints.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 13:39:45 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/26 15:24:02 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_debug_print_arr_ints(int *a, int len, bool lnbk, bool idx, int fd)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (idx)
			ft_dprintf(fd, "index[%i]: ", i);
		ft_dprintf(fd, "%i", a[i]);
		if (lnbk)
			ft_dprintf(fd, "\n");
		else
		{
			if (i != len - 1)
			ft_dprintf(fd, ", ");
		}
		i++;
	}
	ft_dprintf(fd, "\n");
}