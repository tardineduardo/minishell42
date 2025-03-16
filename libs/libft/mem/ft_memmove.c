/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:20:00 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:36:00 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_copy_forwards(unsigned char *d, unsigned char *s, size_t n)
{
	size_t	a;

	a = 0;
	while (a < n)
	{
		*d = *s;
		d++;
		s++;
		a++;
	}
}

static void	ft_copy_backwards(unsigned char *d, unsigned char *s, size_t n)
{
	size_t	a;

	a = 0;
	d += n - 1;
	s += n - 1;
	while (a < n)
	{
		*d = *s;
		d--;
		s--;
		a++;
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*source;
	unsigned char	*destiny;

	source = (unsigned char *)src;
	destiny = (unsigned char *)dest;
	if (src == dest)
		return (dest);
	else if (src > dest)
		ft_copy_forwards(destiny, source, n);
	else if (src < dest)
		ft_copy_backwards(destiny, source, n);
	return (dest);
}
