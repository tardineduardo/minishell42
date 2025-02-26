/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_concatenate_var.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:43 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:36:35 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_concatenate_var(int num, ...)
{
	va_list	args;
	char	*cur_str;
	char	*new_str;
	char	*temp;
	int		a;

	va_start(args, num);
	a = 0;
	new_str = malloc(sizeof(char));
	new_str[0] = 0;
	while (a < num)
	{
		cur_str = va_arg(args, char *);
		if (cur_str == NULL)
			cur_str = "";
		temp = new_str;
		new_str = ft_strjoin(new_str, cur_str);
		free(temp);
		a++;
	}
	va_end(args);
	return (new_str);
}
