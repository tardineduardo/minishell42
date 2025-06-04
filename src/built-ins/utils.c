/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:32 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/03 20:14:49 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/environs.h"

bool	ft_is_valid_varname(char *var, char *variable_value)
{
	int	a;
	int	len;

	if (!ft_isalpha(var[0]) && var[0] != '_')
	{
		ft_dprintf(STDERR_FILENO, "export: `%s`: not a valid identifier\n",
			variable_value);
		return (false);
	}
	a = 1;
	len = ft_strlen(var);
	while (a < len)
	{
		if (!ft_isalnum(var[a]) && var[a] != '_')
		{
			ft_dprintf(STDERR_FILENO, "export: `%s`: not a valid identifier\n",
				variable_value);
			return (false);
		}
		a++;
	}
	return (true);
}

int	ft_error_export(char *variable)
{
	ft_dprintf(2, "export: `%s`: not a valid identifier\n", variable);
	return (EXIT_FAILURE);
}
