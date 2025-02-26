/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:35:42 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:35:53 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			a;
	unsigned char	*array1;
	unsigned char	*array2;

	array1 = (unsigned char *)s1;
	array2 = (unsigned char *)s2;
	a = 0;
	while (a < n)
	{
		if (array1[a] != array2[a])
			return (array1[a] - array2[a]);
		a++;
	}
	return (0);
}
