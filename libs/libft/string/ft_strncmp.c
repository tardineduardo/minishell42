/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:54:03 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:22 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
	{
		return (0);
	}
	while ((n > 1) && (((unsigned char)*s1 == (unsigned char)*s2)) && (*s1))
	{
		s1++;
		s2++;
		n--;
	}
	return (((unsigned char)*s1 - (unsigned char)*s2));
}
