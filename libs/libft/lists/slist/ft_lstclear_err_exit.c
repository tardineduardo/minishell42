/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_err_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:33:44 by eduribei          #+#    #+#             */
/*   Updated: 2024/10/18 15:06:53 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	ft_lstclear_err_exit(t_list **lst, void (*del)(void*), char *e, int n)
{
	t_list	*temp1;
	t_list	*temp2;

	if (lst == NULL || *lst == NULL)
	{
		perror(e);
		exit(n);
	}
	temp1 = *lst;
	while (temp1 != NULL)
	{
		temp2 = temp1->next;
		(*del)(temp1->content);
		free(temp1);
		temp1 = temp2;
	}
	*lst = NULL;
	perror(e);
	exit(n);
}
