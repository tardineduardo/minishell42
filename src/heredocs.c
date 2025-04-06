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

char	*ft_hd_create_file(int *hc_count_int, char **fpath_cap);
char	*ft_hd_input_loop(char **delim, char **fpath_cap, t_list **envlist, t_mem **mem);
bool	ft_hd_break_condition(char *hc_input, char *delim);
void	*ft_hd_write_to_file(char **hc_input, char **fpath_cap, int hc_loop_count);
char	*ft_hd_validate_path(char **fpath_cap, int *hc_count_int);
int		ft_hd_init_file(char **fpath_cap);
void	ft_del_heredoc_node(void *content);


char	*ft_heredoc(t_mem **mem, t_hd_mem **hd, t_list **envlist)
{
	static int	hc_count_int;
	t_hd_node	*hc_node;
	t_list		*hc_new;



	if (!ft_hd_create_file(&hc_count_int, &(*hd)->fpath_cap))
		return (NULL);
	if (!ft_hd_input_loop(&(*hd)->delim, &(*hd)->fpath_cap, envlist, mem))
		return (NULL);
	hc_node = malloc(sizeof(t_hd_node));
	if (!hc_node)
		return (NULL);
	hc_node->fpath_node = (*hd)->fpath_cap;
	hc_new = ft_lstnew(hc_node);
	ft_lstadd_back(&(*hd)->list, hc_new);
	hc_count_int++;
	if (hc_count_int == INT_MAX)
		return (NULL);
	return ((*hd)->delim);
}

//Cria o arquivo temporário para receber o heredoc.
char	*ft_hd_create_file(int *hc_count_int, char **fpath_cap)
{
	char	*hc_count_str;

	hc_count_str = ft_itoa(*hc_count_int);
	if (!hc_count_str)
		return (NULL);
	*fpath_cap = ft_strjoin("ms_temp_heredoc_", hc_count_str);
	ft_free_and_null((void *)&hc_count_str);
	if (!(*fpath_cap))
		return (NULL);
	if (!ft_hd_validate_path(fpath_cap, hc_count_int))
		return (NULL);
	if (ft_hd_init_file(fpath_cap) == -1)
		return (NULL);
	return (*fpath_cap);
}

void	*ft_hd_write_to_file(char **hc_input, char **fpath_cap, int hc_loop_count)
{
	int		hc_temp_file;

	hc_temp_file = open(*fpath_cap, O_WRONLY | O_APPEND, 0644);
	if (hc_temp_file == -1)
	{
		ft_free_and_null((void *)&(*hc_input));
		return (NULL);
	}

	// handle the first line and line breaks
	if (hc_loop_count != 0)
		ft_dprintf(hc_temp_file, "\n");
	if ((*hc_input)[0] == '\0')
		ft_dprintf(hc_temp_file, "\n");
	else
		ft_dprintf(hc_temp_file, "%s", *hc_input);

	close(hc_temp_file);
	ft_free_and_null((void *)&(*hc_input));
	return (*fpath_cap);
}

//simplificar esse prototipo
char	*ft_hd_input_loop(char **delim, char **fpath_cap, t_list **envlist, t_mem **mem)
{
	int			hc_loop_count;
	char		*hc_input;
	char		*temp;

	hc_loop_count = 0;
	while(1)
	{
		hc_input = readline(BLUE "heredoc > " RESET);
		if (!hc_input)
			return (NULL);
		temp = hc_input;
		hc_input = ft_expand_string(hc_input, envlist, mem);
		ft_free_and_null((void *)&temp);
		if (!hc_input)
			return (NULL);
		if (ft_strcmp(*delim, hc_input) == 0)
			break ;
		if (!ft_hd_write_to_file(&hc_input, fpath_cap, hc_loop_count))
			return (NULL);
		hc_loop_count++;
	}
	ft_free_and_null((void *)&hc_input);
	return (*fpath_cap);
}

//Helpers da create file.
char	*ft_hd_validate_path(char **fpath_cap, int *hc_count_int)
{
	char		*hc_count_str;
	char		*hc_temp;

	while (1)
	{
		if (access(*fpath_cap, F_OK) != 0 || access(*fpath_cap, W_OK) == 0)
			break ;
		else
		{
			(*hc_count_int)++;
			hc_count_str = ft_itoa(*hc_count_int);
			if (!hc_count_str)
				return (NULL);
			hc_temp = *fpath_cap;
			*fpath_cap = ft_strjoin("ms_temp_heredoc_", hc_count_str);       
			ft_free_and_null((void *)&hc_count_str);
			if (!(*fpath_cap))
			{
				ft_free_and_null((void *)&hc_temp); // reduzir aqui (juntar no return)
				return (NULL);
			}
			ft_free_and_null((void *)&hc_temp);
		}
	}
	return (*fpath_cap);
}

int	ft_hd_init_file(char **fpath_cap)
{
	int		hc_open_file;

	hc_open_file = open(*fpath_cap, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (hc_open_file == -1)
	{
		ft_dprintf(STDERR_FILENO, "fd error: %s [%i])\n", strerror(errno), errno);
		return (-1);
	}
	close(hc_open_file);
	return (0);
}

void	ft_del_heredoc_node(void *content)
{
	t_hd_node	*hc_node;

	if (!content)
		return ;
	
	hc_node = (t_hd_node *)content;
	if (!hc_node->fpath_node)
		return ;

		
	if (access(hc_node->fpath_node, F_OK) == 0)
	{
		unlink(hc_node->fpath_node);
		ft_free_and_null((void *)&hc_node->fpath_node);
	}
	ft_free_and_null((void *)&hc_node);
}
