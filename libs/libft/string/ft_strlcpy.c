/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:12:35 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:09 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	copy_len;
	size_t	src_len;

	src_len = ft_strlen((char *)src);
	if (size > 0)
	{
		if (src_len == 0)
			copy_len = 0;
		else if (src_len >= size)
			copy_len = size - 1;
		else
			copy_len = src_len;
		ft_memcpy(dest, src, copy_len);
		dest[copy_len] = '\0';
	}
	return (src_len);
}
