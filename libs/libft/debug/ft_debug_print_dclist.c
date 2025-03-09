/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug_print_dclist.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:33:57 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/27 17:42:58 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_print_string(t_dll **tail, t_dll *trav, size_t offset, int fd)
{
	int countlen;

	countlen = 0;
	trav = trav->next;
	while (trav != *tail)
	{
        countlen += ft_dprintf(fd, "[ %s ] <--> ", *(char **)((char *)trav + offset));
        trav = trav->next;
	}
	countlen += ft_dprintf(fd, "[ %s ]", *(char **)((char *)trav + offset));
    ft_dprintf(fd, " <---- tail");
	return (countlen);
}

static int	ft_print_char(t_dll **tail, t_dll *trav, size_t offset, int fd)
{
	int countlen;

	countlen = 0;
	trav = trav->next;
	while (trav != *tail)
	{
        countlen += ft_dprintf(fd, "[ %c ] <--> ", *(char *)((char *)trav + offset));
        trav = trav->next;
	}
	countlen += ft_dprintf(fd, "[ %c ]", *(char *)((char *)trav + offset));
    ft_dprintf(fd, " <---- tail");
	return (countlen);
}

int	ft_print_int(t_dll **tail, t_dll *trav, size_t offset, int fd)
{
	int countlen;

	countlen = 0;
	trav = trav->next;
	ft_dprintf(fd, "\n");
	while (trav != *tail)
	{
        countlen += ft_dprintf(fd, "[ %d ] <--> ", *(int *)((char *)trav + offset));
        trav = trav->next;
	}
	countlen += ft_dprintf(fd, "[ %d ]", *(int *)((char *)trav + offset));
    ft_dprintf(fd, GREEN " <---- " RESET "tail");
	return (countlen);
}

void	ft_print_circle_arrow(t_dll **tail, int countlen, int fd)
{
	int a;
	
	a = ft_dclstsize(tail);

	if(a == 1)
	{
		ft_dprintf(fd, "\n|   |\n");
		ft_dprintf(fd, "+>-<+\n\n");
		return ;
	}		
	ft_dprintf(fd, "\n  |");
	a = 0;
	while (a < countlen - 6)
	{
		ft_dprintf(fd, " ");
		a++;
	}
	ft_dprintf(fd, "|\n");
	ft_dprintf(fd, "  +-<");
	a = 0;
	while (a < countlen - 10)
	{
		ft_dprintf(fd, "-");
		a++;
	}
	ft_dprintf(fd, ">-+\n");
	ft_dprintf(fd, "\n");
}

void	ft_debug_print_dclist(t_dll **tail, char *type, size_t offset, int fd)
{
	int countlen;
	t_dll *trav;

	if (ft_dclstsize(tail) == -1)
		return ;

	countlen = 0;
	trav = *tail;
	if (trav == NULL)
	{
	    ft_dprintf(fd, "\nNULL <---- tail\n\n");
		return ;
	}
	if (ft_strcmp(type, "int") == 0)
		countlen += ft_print_int(tail, trav, offset, fd);
	else if (ft_strcmp(type, "char") == 0)
		countlen += ft_print_char(tail, trav, offset, fd);
	else if (ft_strcmp(type, "string") == 0)
		countlen += ft_print_string(tail, trav, offset, fd);
	else
	    ft_dprintf(fd, "Unsupported data type. Use \"int\", \"char\" or \"string\".\n");
	ft_print_circle_arrow(tail, countlen, fd);
}