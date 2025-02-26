/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atohx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:02:36 by eduribei          #+#    #+#             */
/*   Updated: 2024/12/14 17:43:40 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	to_lower_str(char *s)
{
	int	a;

	a = 0;
	while (s[a])
	{
		s[a] = ft_tolower(s[a]);
		a++;
	}
}

int	hex_value(char c, char *lower)
{
	int	j;

	j = 0;
	while (j < 16)
	{
		if (c == lower[j])
			return (j);
		j++;
	}
	return (-1);
}

t_llong	ft_helper(char *trimm, size_t len)
{
	t_llong	nb;
	int		value;
	char	*lower;

	nb = 0;
	lower = "0123456789abcdef";
	while (len > 0 && *trimm)
	{
		value = hex_value(*trimm, lower);
		if (value == -1)
			return (-1);
		nb = nb * 16 + value;
		trimm++;
		len--;
	}
	return (nb);
}

t_llong	ft_atohx(char *s)
{
	size_t		len;
	char		*trimm;
	t_llong		nb;
	char		*orig_trimm;

	trimm = ft_strdup(s);
	if (!trimm)
		return (-1);
	to_lower_str(trimm);
	len = ft_strlen(trimm);
	if (ft_strnstr(trimm, "0x", 2))
	{
		orig_trimm = trimm;
		trimm += 2;
		len -= 2;
	}
	else
	{
		orig_trimm = trimm;
	}
	nb = ft_helper(trimm, len);
	free(orig_trimm);
	return (nb);
}
