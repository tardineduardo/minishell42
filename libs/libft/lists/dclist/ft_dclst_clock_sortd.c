/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dclst_clock_sortd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:24:19 by eduribei          #+#    #+#             */
/*   Updated: 2024/11/13 16:35:10 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

static bool	ft_check_int_reverse(t_dll **tail, int offset)
{
	t_dll	*trav;
	int		val_min;
	int		val_max;
	int		curr;
	int		next;

	trav = *tail;
	val_min = *(int *)((char *)ft_dclst_find_lo_int(tail, offset) + offset);
	val_max = *(int *)((char *)ft_dclst_find_hi_int(tail, offset) + offset);
	while (1)
	{
		curr = *(int *)((char *)trav + offset);
		next = *(int *)((char *)trav->next + offset);
		if (curr == val_min && next != val_max)
			return (false);
		else if (next != curr - 1)
			return (false);
		trav = trav->next;
		if (trav == *tail)
			break ;
	}
	return (true);
}

static bool	ft_check_int_normal(t_dll **tail, int offset)
{
	t_dll	*trav;
	int		val_min;
	int		val_max;
	int		curr;
	int		next;

	trav = *tail;
	val_min = *(int *)((char *)ft_dclst_find_lo_int(tail, offset) + offset);
	val_max = *(int *)((char *)ft_dclst_find_hi_int(tail, offset) + offset);
	while (1)
	{
		curr = *(int *)((char *)trav + offset);
		next = *(int *)((char *)trav->next + offset);
		if (curr == val_max || next != val_min)
			return (false);
		else if (next != curr + 1)
			return (false);
		trav = trav->next;
		if (trav == *tail)
			break ;
	}
	return (true);
}

/* type i-int | modes n-normal, r-reverse | offssetof(struct, variable)*/
bool	ft_dclst_clock_sortd(t_dll **tail, char mode, int offset)
{
	if (mode == 'n')
		return (ft_check_int_normal(tail, offset));
	else if (mode == 'r')
		return (ft_check_int_reverse(tail, offset));
	else
		ft_perror_exit("Invalid mode.\n", EINVAL);
}
