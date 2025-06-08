/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_control_utils_3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:55:48 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 21:04:32 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parsing.h"
#include "../../include/expand.h"
#include "../../include/execution.h"

int	test_directory(char *expanded_name, t_mem **mem, bool sngl_bi)
{
	struct stat	sb;

	stat(expanded_name, &sb);
	if (S_ISDIR(sb.st_mode))
	{
		if (sngl_bi == true)
		{
			ft_dprintf(2, "%s: Is a directory\n", expanded_name);
			return (126);
		}
		else
			ft_error_handler("%s: Is a directory\n",
				expanded_name, 126, mem);
	}
	return (0);
}

int	test_permission(char *expanded_name, t_mem **mem, bool sngl_bi)
{
	if (sngl_bi == true)
	{
		ft_dprintf(2, "%s: Permission denied\n", expanded_name);
		return (1);
	}
	else
		ft_error_handler("%s: Permission denied\n",
			expanded_name, 1, mem);
	return (0);
}

int	process_input_redir(char *expanded, t_mem **mem, bool sngl_bi)
{
	return (test_input_redir(expanded, mem, sngl_bi));
}

int	process_output_redir(t_redirs_node *redir, char *expanded,
		t_mem **mem, bool sngl_bi)
{
	return (teste_output_redir(expanded, redir->create, mem, sngl_bi));
}

int	process_redirection(t_redirs_node *redir, t_mem **mem, bool sngl_bi)
{
	char	*expanded;
	int		result;

	expanded = ft_expand(&redir->name, TOKEN, mem);
	if (redir->type == IN_R || redir->type == HDC_R)
		result = process_input_redir(expanded, mem, sngl_bi);
	else if (redir->type == OUT_R || redir->type == APPD_R)
		result = process_output_redir(redir, expanded, mem, sngl_bi);
	else
		result = 0;
	return (result);
}
