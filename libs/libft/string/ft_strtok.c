/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:19:39 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/28 20:10:20 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static char	*ft_skip_delimiters(char *input_str, char *delimiters)
{
	int	index;

	index = 0;
	while (input_str[index] != '\0')
	{
		if (!ft_strchr(delimiters, input_str[index]))
			return (&input_str[index]);
		input_str[index] = '\0';
		index++;
	}
	return (NULL);
}

static char	*ft_mark_token_end(char *input_str, char *delimiters)
{
	int	index;

	if (!input_str)
		return (NULL);
	index = 0;
	while (input_str[index] != '\0' && !ft_strchr(delimiters, input_str[index]))
		index++;
	if (input_str[index] == '\0')
		return (NULL);
	input_str[index] = '\0';
	return (&input_str[index + 1]);
}

char	*ft_strtok(char *input_str, char *delimiters)
{
	char		*current_token;
	static char	*next_token_ptr;

	if (!delimiters || delimiters[0] == '\0')
		return (NULL);
	if (!input_str && !next_token_ptr)
		return (NULL);
	else if (input_str)
	{
		current_token = ft_skip_delimiters(input_str, delimiters);
		next_token_ptr = ft_mark_token_end(current_token, delimiters);
	}
	else
	{
		current_token = ft_skip_delimiters(next_token_ptr, delimiters);
		next_token_ptr = ft_mark_token_end(current_token, delimiters);
	}
	return (current_token);
}
