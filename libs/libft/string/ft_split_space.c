/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_space.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:18:57 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/11 10:48:16 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	c_substrs(char *s, char *set)
{
	size_t	count_substrs;
	int		in_substr;

	count_substrs = 0;
	in_substr = 0;
	while (*s != '\0')
	{
		if (!ft_strchr(set, *s) && !in_substr)
		{
			in_substr = 1;
			count_substrs++;
		}
		else if (ft_strchr(set, *s))
			in_substr = 0;
		s++;
	}
	return (count_substrs);
}

static char	*ft_fill(char **results, char *start, size_t count, size_t r_index)
{
	results[r_index] = ft_calloc((count + 1), sizeof(char));
	if (results[r_index] != NULL)
	{
		ft_memmove((char *)results[r_index], (char *)start, count);
		results[r_index][count] = '\0';
		return (results[r_index]);
	}
	else
	{
		ft_free_split(results, r_index);
		return (NULL);
	}
}

static char	**ft_get_substrs(char *s, char *set, char **results)
{
	size_t	substr_len_c;
	size_t	r_index;
	char	*start;

	r_index = 0;
	while (*s != '\0')
	{
		if ((!ft_strchr(set, *s)) && (*s != '\0'))
		{
			start = s;
			substr_len_c = 0;
			while ((!ft_strchr(set, *s)) && (*s != '\0'))
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

char	**ft_split_space(char *s)
{
	char	**results;
	size_t	total_substrs;
	char	*set;

	set = " \t";
	total_substrs = c_substrs((char *)s, set);
	if (total_substrs == 0)
	{
		results = ft_calloc(2, sizeof(char *));
		results[0] = ft_calloc(1, sizeof(char *));
		return (results);
	}
	results = ft_calloc((total_substrs + 1), sizeof(char *));
	if (results == NULL)
		return (NULL);
	results[total_substrs] = NULL;
	results = ft_get_substrs((char *)s, set, results);
	return (results);
}
