/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:06:44 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:37:21 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	a;
	size_t	len;
	char	*newstr;

	a = 0;
	len = ft_strlen(s);
	newstr = malloc(len * sizeof(char) + 1);
	if (newstr == NULL)
		return (NULL);
	while (s[a])
	{
		newstr[a] = (*f)(a, s[a]);
		a++;
	}
	newstr[a] = '\0';
	return (newstr);
}
