/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dclst_dist_head_bidi_len.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:19:15 by eduribei          #+#    #+#             */
/*   Updated: 2024/12/16 13:30:45 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

int	ft_dclst_dist_head_bidi_len(t_dll **tail, t_dll *nd, int list_len)
{
	int		i;
	t_dll	*trav_fwd;
	t_dll	*trav_bck;

	if (!tail || !(*tail))
		return (0);
	i = 0;
	trav_fwd = nd;
	trav_bck = nd;
	while (i < list_len)
	{
		if (trav_fwd == (*tail)->next)
			return (i);
		if (trav_bck == (*tail)->next)
			return (-i);
		i++;
		trav_fwd = trav_fwd->next;
		trav_bck = trav_bck->prev;
	}
	ft_perror_exit("Invalid params: can't find head of the list.\n", EINVAL);
}
