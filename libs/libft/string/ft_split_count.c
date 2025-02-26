/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_count.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:16:06 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/20 12:40:29 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_split_count(char const *s, char c)
{
	size_t	count_substrs;
	int		in_substr;

	count_substrs = 0;
	in_substr = 0;
	while (*s != '\0')
	{
		if (*s != c && !in_substr)
		{
			in_substr = 1;
			count_substrs++;
		}
		else if (*s == c)
			in_substr = 0;
		s++;
	}
	return (count_substrs);
}
