/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard_match.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:30 by luide-ca          #+#    #+#             */
/*   Updated: 2025/06/07 20:32:25 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/tokenize.h"

static bool	ft_match_mid(char *filename, char *token, int lenf)
{
	if (ft_strnstr(filename, token, lenf))
		return (true);
	return (false);
}

static bool	ft_match_sta(char *filename, char *token, int lenp)
{
	if (ft_strncmp(filename, token, lenp) == 0)
		return (true);
	return (false);
}

static bool	ft_match_end(char *filename, char *token, int lenf, int lenp)
{
	char	*substr;
	int		lensub;

	if (lenp > lenf)
		return (false);
	substr = &filename[lenf - lenp];
	lensub = ft_strlen(substr);
	if (ft_strncmp(substr, token, lensub) == 0)
		return (true);
	return (false);
}

static bool	ft_match_edg(char *filename, char *token, int lenf)
{
	char	**edges;

	edges = ft_split_char(token, '*');
	if (ft_match_sta(filename, edges[0], ft_strlen(edges[0]))
		&& ft_match_end(filename, edges[1], lenf, ft_strlen(edges[1])))
	{
		ft_free_str_array(edges);
		return (true);
	}
	ft_free_str_array(edges);
	return (false);
}

bool	ft_is_a_wildcard_match(char *filename, char *token, t_wccase type)
{
	int		lenf;
	int		lenp;
	char	*pattern;
	bool	result;

	if (filename[0] == '.')
		return (false);
	pattern = ft_strtrim(token, "* \t");
	if (!pattern)
		return (false);
	lenf = ft_strlen(filename);
	lenp = ft_strlen(pattern);
	result = false;
	if (type == PAT_STR)
		result = ft_match_sta(filename, pattern, lenp);
	else if (type == PAT_END)
		result = ft_match_end(filename, pattern, lenf, lenp);
	else if (type == PAT_MID)
		result = ft_match_mid(filename, pattern, lenf);
	else if (type == PAT_EDG)
		result = ft_match_edg(filename, token, lenf);
	else if (type == LIST_ALL)
		result = true;
	free(pattern);
	return (result);
}
