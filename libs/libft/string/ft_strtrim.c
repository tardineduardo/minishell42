/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:55:10 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:24 by eduribei         ###   ########.fr       */
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
	size_t	copy_len;
	char	*trimmed;
	char	*start;
	char	*end;

	len = ft_strlen(s1);
	start = ft_find_start((char *)s1, (char *)set, len);
	end = ft_find_end((char *)s1, (char *)set, len);
	if (start > end)
	{
		trimmed = malloc(sizeof(char));
		if (trimmed == NULL)
			return (NULL);
		trimmed[0] = '\0';
		return (trimmed);
	}
	else
	{
		copy_len = end - start;
		trimmed = malloc((copy_len + 1) * sizeof(char) + 1);
		if (trimmed == NULL)
			return (NULL);
		return (ft_copy(trimmed, start, copy_len));
	}
}
