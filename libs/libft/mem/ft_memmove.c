/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:24:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/02/28 15:25:12 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*ptr_src;
	unsigned char		*ptr_dest;
	size_t				i;

	ptr_src = (const unsigned char *)src;
	ptr_dest = (unsigned char *)dest;
	if (ptr_dest == ptr_src || n == 0)
		return (dest);
	i = 0;
	if (ptr_dest < ptr_src)
	{
		while (n > i)
		{
			ptr_dest[i] = ptr_src[i];
			i++;
		}
	}
	else
	{
		while (0 < n--)
			ptr_dest[n] = ptr_src[n];
	}
	return (dest);
}