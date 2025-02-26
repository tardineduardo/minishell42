/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_concatenate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:43 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/26 14:57:31 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_concatenate(char *s1, char *s2, char *s3)
{
	char	*new_str;
	char	*temp;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";
	if (s3 == NULL)
		s3 = "";
	new_str = ft_calloc(1, sizeof(char));
	temp = new_str;
	new_str = ft_strjoin(new_str, s1);
	free(temp);
	temp = new_str;
	new_str = ft_strjoin(new_str, s2);
	free(temp);
	temp = new_str;
	new_str = ft_strjoin(new_str, s3);
	free(temp);
	return (new_str);
}
