/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dclst_list_sorted.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:23:56 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/14 19:27:05 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

bool	ft_dclst_list_sortd(t_dll **tail, t_accessor get_label)
{
	int		len;
	int		curr;
	int		next;
	t_dll	*trav;

	len = ft_dclstsize(tail);
	trav = (*tail)->next;
	if (len < 0)
		ft_perror_exit("ft_dclstsize error", EINVAL);
	if (len == 1 || len == 0)
		return (true);
	while (len > 1)
	{
		curr = *(int *)get_label(trav);
		next = *(int *)get_label(trav->next);
		if (curr > next)
			return (false);
		trav = trav->next;
		len--;
	}
	return (true);
}
