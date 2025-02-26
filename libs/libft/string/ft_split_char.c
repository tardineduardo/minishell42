/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:16:06 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/28 15:18:07 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	c_substrs(char *s, char c)
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

static void	ft_free_if_error(char **results, size_t r_index)
{
	size_t	a;

	a = 0;
	while (a < r_index)
	{
		free(results[a]);
		a++;
	}
	free(results);
}

static char	*ft_fill(char **results, char *start, size_t count, size_t r_index)
{
	results[r_index] = ft_calloc((count + 1), sizeof(char));
	if (results[r_index] == NULL)
	{
		ft_free_if_error(results, r_index);
		return (NULL);
	}
	ft_memmove((char *)results[r_index], (char *)start, count);
	return (results[r_index]);
}

static char	**ft_get_substrs(char *s, char c, char **results)
{
	size_t	substr_len_c;
	size_t	r_index;
	char	*start;

	r_index = 0;
	while (*s != '\0')
	{
		if ((*s != c) && (*s != '\0'))
		{
			start = s;
			substr_len_c = 0;
			while ((*s != c) && (*s != '\0'))
			{
				substr_len_c++;
				s++;
			}
			if (ft_fill(results, start, substr_len_c, r_index))
				r_index++;
			else
				return (NULL);
		}
		else
			s++;
	}
	return (results);
}

char	**ft_split_char(char const *s, char c)
{
	char	**results;
	size_t	total_substrs;

	total_substrs = c_substrs((char *)s, c);
	results = ft_calloc((total_substrs + 1), sizeof(char *));
	if (results == NULL)
		return (NULL);
	results = ft_get_substrs((char *)s, c, results);
	return (results);
}
