/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operators.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/06/01 00:48:41 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

static bool	ft_check_success(t_tok_mem **tok)
{
	int		a;

	a = 0;
	while (a < 14)
	{
		if (!(*tok)->operators[a])
			return (false);
		a++;
	}
	return (true);
}

void	*ft_init_operators(t_tok_mem **tok)
{
	(*tok)->operators = malloc(15 * sizeof(char *));
	if (!(*tok)->operators)
		return (NULL);
	(*tok)->operators[0] = ft_strdup("<<<");
	(*tok)->operators[1] = ft_strdup(">>");
	(*tok)->operators[2] = ft_strdup("<<");
	(*tok)->operators[3] = ft_strdup("&&");
	(*tok)->operators[4] = ft_strdup("||");
	(*tok)->operators[5] = ft_strdup("2>");
	(*tok)->operators[6] = ft_strdup("&>");
	(*tok)->operators[7] = ft_strdup("<");
	(*tok)->operators[8] = ft_strdup(">");
	(*tok)->operators[9] = ft_strdup("|");
	(*tok)->operators[10] = ft_strdup("&");
	(*tok)->operators[11] = ft_strdup("*");
	(*tok)->operators[12] = ft_strdup("(");
	(*tok)->operators[13] = ft_strdup(")");
	(*tok)->operators[14] = NULL;
	if (!ft_check_success(tok))
		return (NULL);
	return (tok);
}
