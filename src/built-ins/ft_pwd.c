/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:07:47 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/14 13:28:14 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"  
#include "../../include/heredoc.h"
#include "../../include/expand.h"
#include "../../include/tokenize.h"
#include "../../include/environs.h"
#include "../../include/heredoc.h"
#include "../../include/builtins.h"

/*
	**Mandatory: pwd with no options***
	PWD just print the full filename  of the current
	working directory. Strict to the point.

	Implementation: the program iterates throught
	our linked list until it finds PWD and print it without
	variable name PWD or '=' operator.
*/

//@luiscarvalhofrade eu estava estudando o export e fazendo uns testes com 
//a nova environs.c e descobri que o bash reseta a variável PWD toda vez
//que é inicializado usando a função getcwd(). Eu estava testando o que
//acontecia quando o Minishell tinha que inicializar sem nenhuma variável
//herdada do parent (rodando "env -i ./minishell"). Daí mesmo isso seja feito,
//o bash tem pelos menos a PWD 

int	ft_pwd(t_list **envlist)
{
	t_list		*trav;
	t_env_node	*current;

	trav = *envlist;
	while (trav)
	{
		current = (t_env_node *)trav->content;
		if (ft_strcmp(current->variable, "PWD") == 0)
		{
			ft_printf("%s\n", current->value);
			break ;
		}
		trav = trav->next;
	}
	return (0);
}
