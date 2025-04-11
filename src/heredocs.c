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

char	*ft_hd_create_file(int *hd_count_int, char **filepath);
char	*ft_hd_input_loop(t_list **envlist, t_mem **mem);
void	*ft_hd_write_to_file(int hd_loop_count, t_mem **mem);
char	*ft_hd_validate_path(char **filepath, int *hd_count_int);
int		ft_hd_init_file(char **filepath);
void	ft_del_heredoc_node(void *content);
void	ft_reset_exp(t_mem **mem);


char	*ft_heredoc(char *delimiter, t_mem **mem)
{
	static int	hd_count_int;
	t_hd_mem	*hd;
	t_env_mem	*env;

	hd = (*mem)->heredoc;
	env = (*mem)->environs;
	hd->delim = ft_exp_hd_delim(delimiter, mem);
	if (!hd->delim)
		return (NULL);
	if (!ft_hd_create_file(&hd_count_int, &hd->filepath))
		return (NULL);
	if (!ft_hd_input_loop(&env->envlist, mem))
		return (NULL);
	hd->node = malloc(sizeof(t_hd_node));
	if (!hd->node)
		return (NULL);
	hd->node->fpath = hd->filepath;
	hd->new = ft_lstnew(hd->node);
	ft_lstadd_back(&hd->list, hd->new);
	hd_count_int++;
	if (hd_count_int == INT_MAX)
		return (NULL);
	return (hd->delim);
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
	t_hd_mem	*hd; 

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
	t_hd_mem	*hd; 

	hd =(*mem)->heredoc;
	(void)envlist;
	(void)mem;
	hd_loop_count = 0;
	while(1)
	{
		//Essa parte aqui ajuda a visualizar qual o delimitador esperado.
		char *prompt = ft_concatenate("heredoc [", hd->delim, "] >");
		ft_printf(BLUE "");
		hd->loopinput = readline(prompt);
		free(prompt);
		ft_printf(RESET "");
		if (!hd->loopinput)
			return (NULL);
		hd->looptemp = hd->loopinput;
//		ft_free_and_null((void *)&hd->looptemp);
		if (!hd->loopinput)
			return (NULL);

		// Primeiro compara, SÓ DEPOIS expande.
		if (ft_strcmp(hd->delim, hd->loopinput) == 0)
			break ;

		//Aqui tem um leak, depois arrumo. 
		hd->loopinput = ft_exp_hd_input(hd->loopinput, mem);

		if (hd->loopinput)
			if (!ft_hd_write_to_file(hd_loop_count, mem))
				return (NULL);
		hd_loop_count++;
	}
	ft_free_and_null((void *)&hd->loopinput);
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

void	ft_del_heredoc_node(void *content)
{
	t_hd_node	*hd_node;

	if (!content)
		return ;
	
	hd_node = (t_hd_node *)content;
	if (!hd_node->fpath)
		return ;

		
	if (access(hd_node->fpath, F_OK) == 0)
	{
		unlink(hd_node->fpath);
		ft_free_and_null((void *)&hd_node->fpath);
	}
	ft_free_and_null((void *)&hd_node);
}



