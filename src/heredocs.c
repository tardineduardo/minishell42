/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 16:24:03 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_hc_create_file(int *hc_count_int, char **fpath_cap);
char	*ft_hc_input_loop(char **delim, char **fpath_cap);

// ft_hc_create_file helpers
bool	ft_hc_break_condition(char *hc_input, char *delim);
void	*ft_write_to_file(char **hc_input, char **fpath_cap, int hc_loop_count);
char	*ft_hc_validate_path(char **fpath_cap, int *hc_count_int);
int		ft_hc_init_file(char **fpath_cap);

// errors
void	*ft_hd_syscall_error(char *message);
void	ft_del_heredoc_node(void *content);
void	*ft_hc_cap_error(char *message, t_hd_mem **hd);

/*
Esta função serve para capturar input do usuário quando o operador << é usado.
Ela recebe como argumentos a string delim e o pointer para a o head da
linked list hd_list.

O valor de retorno fpath_cap serve apenas para indicar o sucesso da operação.
Se houver qualquer erro na captura, o valor de retorno é NULL. Em caso de
erro, toda a memória até então alocada e o prompt devolvido para o usuário.
*/

char	*ft_heredoc(t_hd_mem **hd)
{
	static int	hc_count_int;
	t_hd_node	*hc_node;
	t_list		*hc_new;

	assert(*hd);
	assert(hd);
	
		/* primeiro vamos criar a string do file path onde vamos criar o
		arquivo onde vamos salvar os comandos digitados no heredoc. */
	if (!ft_hc_create_file(&hc_count_int, &(*hd)->fpath_cap))
		return (ft_hc_cap_error("hc_capture: path error\n", hd));

		/* depois iniciamos a captura dos de cada comando em um loop */
	if (!ft_hc_input_loop(&(*hd)->delim, &(*hd)->fpath_cap))
		return (ft_hc_cap_error("hc_capture: loop error\n", hd));

		/* Sucesso? Então vamos criar um node de t_hd_nodes para armazenar
		o caminho do arquivo que vamos abrir na fase de execução. A cada vez
		que a ft_heredoc é chamada, um node é acrescentado na lista. 
		Quando terminarmos de executar todos os comandos, precisamos limpar
		a lista de heredos, assim como a ft_hc_cap_error já faz em caso
		de erro.  */

	hc_node = malloc(sizeof(t_hd_node));
	if (!hc_node)
		return (ft_hc_cap_error("node malloc error\n", hd));
	hc_node->fpath_node = (*hd)->fpath_cap;
	hc_new = ft_lstnew(hc_node);

	/* Chamamos a função da libft para incluir um node no fim da lista. 
		Para usarmos a função da libft, a gente precisa fazer typecasting
		toda hora que vamos mandar manda um node para lista ou ler o 
		content de um node que pegamos da lista (ver como isso é feito
		também na ft_del_heredoc_node) */
	ft_lstadd_back(&(*hd)->list, hc_new);

		/* Incrementamos o static count. Quando a ft_heredoc for chamada
		de novo, o contador não zero. Depois preciso dar um jeito de
		zerar esse contador depois da fase de execução - ou vou precisar
		retirar minha verificação de maxint dos erros... */
	hc_count_int++;
	if (hc_count_int == INT_MAX)
		return (ft_hc_cap_error("node limit error\n", hd));
	
		/* Não vamos usar esse valor de retorno. Na fase de execução vamos
		acessar direto a t_list **hd_list. Mas o valor de retorno está aí
		para a que a função caller tenha como conferir o sucesso do heredoc.
		Eu fazia muita função void, mas isso é ruim para verificar erros.*/ 
	// ft_free_and_null((void *)&(*hd)->fpath_cap);
	return ((*hd)->delim);
}





/******************************************************************************* 

Cria o arquivo temporário para receber o heredoc.

*******************************************************************************/

char	*ft_hc_create_file(int *hc_count_int, char **fpath_cap)
{
	char	*hc_count_str;

	assert(hc_count_int);
	assert(fpath_cap);

	hc_count_str = ft_itoa(*hc_count_int);
	if (!hc_count_str)
		return (ft_hd_syscall_error("hc_create_file - itoa error"));

	*fpath_cap = ft_strjoin("ms_temp_heredoc_", hc_count_str);
	ft_free_and_null((void *)&hc_count_str);
	if (!(*fpath_cap))
		return (ft_hd_syscall_error("hc_create_file - strjoin error"));

	if (!ft_hc_validate_path(fpath_cap, hc_count_int))
		return (NULL);

	if (ft_hc_init_file(fpath_cap) == -1)
		return (NULL);


	return (*fpath_cap);
}



/******************************************************************************* 

O loop de captura para cada linha do readline.

*******************************************************************************/


bool	ft_hc_break_condition(char *hc_input, char *delim)
{
	assert(hc_input);
	assert(delim);

	if (ft_strcmp(delim, hc_input) == 0)
		return (true);

	return (false);
}

void	*ft_write_to_file(char **hc_input, char **fpath_cap, int hc_loop_count)
{
	int		hc_temp_file;

	assert(hc_input);
	assert(fpath_cap);

	hc_temp_file = open(*fpath_cap, O_WRONLY | O_APPEND, 0644);
	if (hc_temp_file == -1)
	{
		ft_free_and_null((void *)&(*hc_input));
		return (ft_hd_syscall_error("heredoc open error"));
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


char	*ft_hc_input_loop(char **delim, char **fpath_cap)
{
	int			hc_loop_count;
	char		*hc_input;

	assert(delim);
	assert(fpath_cap);

	hc_loop_count = 0;
	while(1)
	{
		hc_input = readline("> ");
		if (!hc_input)
			return (ft_hd_syscall_error("heredoc readline error"));

		if (ft_hc_break_condition(hc_input, *delim))
			break ;
		
		if (!ft_write_to_file(&hc_input, fpath_cap, hc_loop_count))
			return (NULL);
		hc_loop_count++;
	}
	ft_free_and_null((void *)&hc_input);
	return (*fpath_cap);
}



/******************************************************************************* 

Helpers da create file.

*******************************************************************************/

char	*ft_hc_validate_path(char **fpath_cap, int *hc_count_int)
{
	char		*hc_count_str;
	char		*hc_temp;

	assert(fpath_cap);
	assert(hc_count_int);

	while (1)
	{
		if (access(*fpath_cap, F_OK) != 0 || access(*fpath_cap, W_OK) == 0)
			break ;

		else
		{
			(*hc_count_int)++;
			hc_count_str = ft_itoa(*hc_count_int);
			if (!hc_count_str)
				return (ft_hd_syscall_error("itoa error"));
			hc_temp = *fpath_cap;
			*fpath_cap = ft_strjoin("ms_temp_heredoc_", hc_count_str);       
			ft_free_and_null((void *)&hc_count_str);
			if (!(*fpath_cap))
			{
				ft_free_and_null((void *)&hc_temp);
				return (ft_hd_syscall_error("strjoin error"));
			}
			ft_free_and_null((void *)&hc_temp);
		}
	}
	return (*fpath_cap);
}


int	ft_hc_init_file(char **fpath_cap)
{
	int		hc_open_file;

	assert(fpath_cap);
	assert(*fpath_cap);

	hc_open_file = open(*fpath_cap, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (hc_open_file == -1)
	{
		ft_dprintf(STDERR_FILENO, "fd error: %s [%i])\n", strerror(errno), errno);
		return (-1);
	}
	close(hc_open_file);
	return (0);
}




/******************************************************************************* 

Funções para pegar erro.

*******************************************************************************/

void	*ft_hd_syscall_error(char *message)
{
	ft_dprintf(STDERR_FILENO, "%s: %s [%i]\n", message, strerror(errno), errno);
	return (NULL);
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

void	*ft_hc_cap_error(char *message, t_hd_mem **hd)
{
	ft_dprintf(STDERR_FILENO, "%s", message);

	if (access((*hd)->fpath_cap, F_OK))
		unlink((*hd)->fpath_cap);
	if ((*hd)->fpath_cap)
		ft_free_and_null((void *)&(*hd)->fpath_cap);

	ft_lstclear(&(*hd)->list, ft_del_heredoc_node);

	return (NULL);
}
