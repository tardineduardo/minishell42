/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:21:48 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:34:47 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_get_digits(char *temp, unsigned int ft_abs, int n)
{
	int	a;

	a = 0;
	while (ft_abs >= 1)
	{
		temp[a] = (ft_abs % 10) + 48;
		ft_abs = ft_abs / 10;
		a++;
	}
	if (n < 0)
		temp[a] = '-';
}

static void	ft_copy_reverse(char *temp, char *number)
{
	int	max;
	int	a;

	a = 0;
	max = ft_strlen(temp) - 1;
	while (max >= 0)
	{
		number[max] = temp[a];
		a++;
		max--;
	}
}

char	*ft_itoa(int n)
{
	unsigned int	ft_abs;
	char			temp[12];
	char			*number;

	ft_bzero(temp, 12);
	if (n == 0)
	{
		number = malloc(2);
		if (number == NULL)
			return (NULL);
		number[0] = '0';
		number[1] = '\0';
		return (number);
	}
	else if (n < 0)
		ft_abs = (unsigned int)(-n);
	else
		ft_abs = n;
	ft_get_digits(temp, ft_abs, n);
	number = ft_calloc((ft_strlen(temp) + 1), sizeof(char));
	if (number == NULL)
		return (NULL);
	ft_copy_reverse(temp, number);
	return (number);
}
