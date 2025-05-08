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

	// handle the first line and line breaks
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
		char *prompt = ft_concatenate("heredoc [", hd->delim, "] > ");
		hd->loopinput = ft_capture_in_interactive_mode(prompt);
		free(prompt);
		if (!hd->loopinput)
			return (NULL);
			
		// Primeiro compara, SÓ DEPOIS expande.
		if (ft_strcmp(hd->delim, hd->loopinput) == 0)
			break ;

		hd->loopinput = ft_expand(&hd->loopinput, HEREDOC, mem);

		if (hd->loopinput)
			if (!ft_hd_write_to_file(hd_loop_count, mem))
				return (NULL);
		hd_loop_count++;
	}
	ft_free_and_null((void *)&hd->loopinput);

	(*mem)->expand->hd_mode = EXPAND; ///////////////////////////////////////////////////////////TIRAR ISSO DAQUI E LEVAR PARA DENTRO DE EXPAND


	return (hd->filepath);
}

//Helpers da create file.
char	*ft_hd_validate_path(char **filepath, int *hd_count_int)
{
	char		*hd_count_str;
	char		*hd_temp;

	while (1)
	{
		if (access(*filepath, F_OK) != 0 || access(*filepath, W_OK) == 0)
			break ;
		else
		{
			(*hd_count_int)++;
			hd_count_str = ft_itoa(*hd_count_int);
			if (!hd_count_str)
				return (NULL);
			hd_temp = *filepath;
			*filepath = ft_strjoin("ms_temp_heredoc_", hd_count_str);       
			ft_free_and_null((void *)&hd_count_str);
			if (!(*filepath))
			{
				ft_free_and_null((void *)&hd_temp); // reduzir aqui (juntar no return)
				return (NULL);
			}
			ft_free_and_null((void *)&hd_temp);
		}
	}
	return (*filepath);
}

int	ft_hd_init_file(char **filepath)
{
	int		hd_open_file;

	hd_open_file = open(*filepath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (hd_open_file == -1)
	{
		ft_dprintf(STDERR_FILENO, "fd error: %s [%i])\n", strerror(errno), errno);
		return (-1);
	}
	close(hd_open_file);
	return (0);
}


void	*ft_init_hdc_memory(t_mem **mem)
{
	(*mem)->heredoc = malloc(sizeof(t_hdc_mem));
	if(!(*mem)->heredoc)
		return (NULL);
	(*mem)->heredoc->delim = NULL;
	(*mem)->heredoc->filepath = NULL;
	(*mem)->heredoc->looptemp = NULL;
	(*mem)->heredoc->loopinput = NULL;
	(*mem)->heredoc->mode = INIT_MODE;
	return ((*mem)->heredoc);
}

void	ft_clear_hdc_mem(t_hdc_mem **hd)
{
	ft_free_and_null((void *)&(*hd)->delim);
	free(*hd);
	return ;
}
