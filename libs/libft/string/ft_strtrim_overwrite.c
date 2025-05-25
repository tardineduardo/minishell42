/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_overwrite.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 10:01:55 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:27 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_is_set(char a, char *set)
{
	while (*set)
	{
		if (*set == a)
			return (1);
		set++;
	}
	return (0);
}

static char	*ft_find_start(char *s, char *set, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len && ft_is_set(s[i], set))
		i++;
	return (&s[i]);
}

static char	*ft_find_end(char *s, char *set, size_t len)
{
	size_t	i;

	i = len;
	while (i > 0 && ft_is_set(s[i - 1], set))
		i--;
	return (&s[i - 1]);
}

char	*ft_strtrim_overwrite(char **s, char *set)
{
	size_t	len;
	size_t	new_len;
	char	*start;
	char	*end;
	char	*s1;

	s1 = *s;
	if (!s1 || !set)
		return (NULL);
	if (*s1 == '\0')
		return (s1);
	len = ft_strlen(s1);
	start = ft_find_start(s1, set, len);
	end = ft_find_end(s1, set, len);
	if (start > end)
	{
		*s1 = '\0';
		return (s1);
	}
	new_len = end - start + 1;
	ft_memmove(s1, start, new_len);
	s1[new_len] = '\0';
	return (s1);
}
