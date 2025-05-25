/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:55:10 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/06 22:07:53 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_is_set(char a, char *set)
{
	while (*set != '\0')
	{
		if (*set == a)
			return (1);
		set++;
	}
	return (0);
}

static char	*ft_find_start(char *s, char *set, size_t len)
{
	size_t	a;
	char	*start;

	a = 0;
	while (a < len)
	{
		if (ft_is_set(s[a], set))
			a++;
		else
		{
			start = &s[a];
			return (start);
		}
	}
	return (&s[len]);
}

static char	*ft_find_end(char *s, char *set, size_t len)
{
	size_t	a;
	char	*end;

	a = len - 1;
	while (a != 0)
	{
		if (ft_is_set(s[a], set))
			a--;
		else
		{
			end = &s[a];
			return (end);
		}
	}
	return (&s[0]);
}

static char	*ft_copy(char *trimmed, char *start, size_t copy_len)
{
	size_t	a;

	a = 0;
	while (a < copy_len + 1)
	{
		trimmed[a] = start[a];
		a++;
	}
	trimmed[a] = '\0';
	return (trimmed);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	char	*trimmed;
	char	*start;
	char	*end;

	if (!s1 || s1[0] == '\0')
		return (ft_calloc(1, sizeof(char)));
	len = ft_strlen(s1);
	start = ft_find_start((char *)s1, (char *)set, len);
	end = ft_find_end((char *)s1, (char *)set, len);
	if (start > end)
	{
		trimmed = ft_calloc(1, sizeof(char));
		if (trimmed == NULL)
			return (NULL);
		return (trimmed);
	}
	else
	{
		trimmed = malloc((end - start + 1) * sizeof(char) + 1);
		if (trimmed == NULL)
			return (NULL);
		return (ft_copy(trimmed, start, end - start));
	}
}
