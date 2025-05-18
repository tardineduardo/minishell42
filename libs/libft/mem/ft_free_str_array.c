/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:43 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:35:45 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_free_str_array(char **array_of_strings)
{
	int	a;

	if (!(*array_of_strings))
		return ;
	a = 0;
	while ((array_of_strings)[a])
	{
		if (array_of_strings[a])
			free(array_of_strings[a]);
		a++;
	}
	free(array_of_strings);
}
