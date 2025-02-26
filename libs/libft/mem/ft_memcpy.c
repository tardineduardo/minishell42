/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:35:20 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:35:57 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			a;
	unsigned char	*source;
	unsigned char	*destiny;

	source = (unsigned char *)src;
	destiny = (unsigned char *)dest;
	a = 0;
	while (a < n)
	{
		destiny[a] = source[a];
		a++;
	}
	return (dest);
}
