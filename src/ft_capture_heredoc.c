/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/02/21 21:16:14 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool		ft_hc_validate_path(char *hc_filepath, int *hc_count_int);
static char		*ft_hc_create_filepath(int *hc_count_int, char *hc_filepath);
void 			*ft_hc_node_malloc_error(char *hc_filepath, t_list **heredocs);
void 			*ft_hc_filepath_error(void);
void			*ft_hc_path_validation_error(char *turnnull);
void 			*ft_hc_loop_error(void);

/*
ft_capture_heredoc()
Essa função não precisaria ter um valor de retorno. Ela poderia apenas
capturar o heredoc e adicioná-lo a uma lista. Quando for a hora de 
acessar o contúdo de cada heredoc, começamos a percorrer a lista.

Aqui o path é retornado aqui APENAS para que a função caller verifique
o sucesso da operação. Se houver qualquer erro na captura, o valor de
retorno é NULL. Em caso de erro, liberar toda a memória até então alocada
e devolver o prompt para o usuário.
*/

char	*ft_capture_heredoc(char *hc_delimiter, t_list **heredocs)
{
	static int	hc_count_int;
	t_heredoc	*hc_node;
	char		*hc_filepath;
	bool		*hc_success;

	hc_success = NULL;

	ft_hc_create_filepath(&hc_count_int, hc_filepath);
	if (!hc_filepath)
		return (ft_hc_filepath_error());

	ft_start_capture_input_loop(hc_delimiter, hc_filepath, hc_success);
	if (!hc_success)
		return (ft_hc_loop_error());

	hc_node = malloc(sizeof(t_heredoc));
	if (!hc_node);
		return (ft_hc_node_malloc_error(hc_filepath, heredocs));
	
	hc_node->heredoc_file = hc_filepath;
	ft_lstadd_back(heredocs, hc_node);
	hc_count_int++;
	if (hc_count_int == INT_MAX)
		return (ft_hc_int_max_error(heredocs));
	return (hc_filepath);
}

static char *ft_hc_create_filepath(int *hc_count_int, char *hc_filepath)
{
	char	*hc_count_str;

	hc_count_str = ft_itoa(*hc_count_int);
	if (!hc_count_str)
		return (NULL);
	
	hc_filepath = ft_strjoin("/tmp/ms_temp_heredoc_", hc_count_str);
	if (!hc_filepath)
	{
		free(hc_count_str);
		return (NULL);
	}
	free(hc_count_str);

	ft_hc_validate_path(hc_filepath, hc_count_int);
	if (!hc_filepath);
		return(ft_hc_validate_path_error());

	return(hc_filepath);
}

static bool	ft_hc_validate_path(char *hc_filepath, int *hc_count_int)
{
	char		*hc_count_str;
	char		*hc_temp;

	while (1)
	{
		if (access(hc_filepath, W_OK))
			break ;
		else
		{
			*hc_count_int++;
			hc_count_str = ft_itoa(*hc_count_int);
			if (!hc_count_str)
				return (ft_hc_path_validation_error(hc_filepath));	//free filpath, set to null	
			hc_temp = hc_filepath;
			hc_filepath = ft_strjoin("/tmp/minish_temp_heredoc_", hc_count_str); //free countstr
			if (!hc_filepath)
				return (ft_hc_path_validation_error(hc_temp));			
			if (access(hc_filepath, W_OK))
				break ;			
		}
	}
	return (true);
}



ft_start_capture_input_loop(hc_delimiter, hc_filepath, hc_success)
{



}






void 			*ft_hc_filepath_error(void)
{
	// ft_dprintf(STDERR_FILENO, "Error assigning heredoc filepath: %s (%i)", strerror(errno), errno);
}

void 			*ft_hc_loop_error(void)
{
	// {
	// 	ft_dprintf(STDERR_FILENO, "Error capturing heredoc: %s (%i)", strerror(errno), errno);
	// 	return (NULL);
	// }

}


void 			*ft_hc_node_malloc_error(char *hc_filepath, t_list **heredocs)
{
	// malloc error:
	// 		//delete_current_file
	// 	//delete_old_files
	// 	//free_filepath
	// 	return (NULL);
}
