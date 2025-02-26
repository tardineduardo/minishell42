/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:54:03 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:23 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strrchr(const char *str, int c)
{
	unsigned char	c_usc;
	int				a;
	int				len;
	char			*s;

	c_usc = (unsigned char)c;
	len = ft_strlen((char *)str);
	s = (char *)str;
	a = len;
	while (a >= 0)
	{
		if (s[a] == c_usc)
			return (&s[a]);
		a--;
	}
	return (NULL);
}
