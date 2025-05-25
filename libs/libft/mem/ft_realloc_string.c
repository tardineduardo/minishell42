/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_string.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 10:01:55 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:27 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_realloc_string(char **string, size_t newlen)
{
	char	*new;

	if (!string || !(*string))
		return (NULL);
	new = ft_calloc(newlen, 1);
	if (!new)
		return (NULL);
	ft_strlcpy(new, *string, newlen + 1);
	ft_free_and_null((void *)&(*string));
	*string = new;
	return (new);
}
