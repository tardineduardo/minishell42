/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strremove_set.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 10:01:55 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:27 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strremove_set(char *str, char *set)
{
	size_t	a;
	size_t	b;
	char	*new;
	char	*temp;

	if (!str || !set)
		return (NULL);
	new = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new)
		return (NULL);
	a = 0;
	b = 0;
	while (str[a])
	{
		if (!ft_strchr(set, str[a]))
			new[b++] = str[a];
		a++;
	}
	temp = new;
	new = ft_strdup(new);
	if (!new)
		return (free(temp), NULL);
	free(temp);
	return (new);
}
