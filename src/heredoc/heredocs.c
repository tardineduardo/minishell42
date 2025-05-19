/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/30 19:15:02 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/expand.h"

char	*ft_heredoc(char *delimiter, t_mem **mem)
{
	static int	hd_count_int;
	t_hdc_mem	*hd;
	t_env_mem	*env;

	hd = (*mem)->heredoc;
	env = (*mem)->environs;

	hd->delim = ft_strdup(delimiter);
	ft_expand(&hd->delim, DELIMITER, mem);
	if (!hd->delim)
		return (NULL);
	if (!ft_hd_create_file(&hd_count_int, &hd->filepath))
		return (NULL);
	if (!ft_hd_input_loop(&env->envlist, mem))
		return (NULL);
	hd_count_int++;
	if (hd_count_int == INT_MAX)
		return (NULL);
	ft_free_and_null((void *)&hd->delim);
	return (hd->filepath);
}


//Cria o arquivo temporário para receber o heredoc.
char	*ft_hd_create_file(int *hd_count_int, char **filepath)
{
	char	*hd_count_str;

	hd_count_str = ft_itoa(*hd_count_int);
	if (!hd_count_str)
		return (NULL);
	*filepath = ft_strjoin("ms_temp_heredoc_", hd_count_str);
	ft_free_and_null((void *)&hd_count_str);
	if (!(*filepath))
		return (NULL);
	if (!ft_hd_validate_path(filepath, hd_count_int))
		return (NULL);
	if (ft_hd_init_file(filepath) == -1)
		return (NULL);
	return (*filepath);
}

void	*ft_hd_write_to_file(int hd_loop_count, t_mem **mem)
{
	int			hd_temp_file;
	t_hdc_mem	*hd; 

	hd = (*mem)->heredoc;

	hd_temp_file = open(hd->filepath, O_WRONLY | O_APPEND, 0644);
	if (hd_temp_file == -1)
	{
		ft_free_and_null((void *)&(hd->loopinput));
		return (NULL);
	}
	if (hd_loop_count != 0)
		ft_dprintf(hd_temp_file, "\n");
	if ((hd->loopinput)[0] == '\0')
		ft_dprintf(hd_temp_file, "\n");
	else
		ft_dprintf(hd_temp_file, "%s", hd->loopinput);

	close(hd_temp_file);
	ft_free_and_null((void *)&hd->loopinput);
	return (hd->filepath);
}

//simplificar esse prototipo
char	*ft_hd_input_loop(t_list **envlist, t_mem **mem)
{
	int			hd_loop_count;
	t_hdc_mem	*hd;
	
	hd =(*mem)->heredoc;
	(void)envlist;
	hd_loop_count = 0;
	while(1)
	{
		hd->loopinput = ft_capture_in_interactive_mode(makeprompt(hd->delim));
		if (!hd->loopinput)
			return (NULL);
		if (ft_strcmp(hd->delim, hd->loopinput) == 0)
			break ;

		hd->loopinput = ft_expand(&hd->loopinput, HEREDOC, mem);

		if (hd->loopinput)
			if (!ft_hd_write_to_file(hd_loop_count, mem))
				return (NULL);
		hd_loop_count++;
	}
	ft_free_and_null((void *)&hd->loopinput);
	(*mem)->expand->hd_mode = EXPAND;
	return (hd->filepath);
}

char	*makeprompt(char *delim)
{
}


