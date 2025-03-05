/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_capture_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/01 19:46:30 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_hc_create_file(int *hc_count_int, char **hc_fpath);
char	*ft_hc_input_loop(char *hc_delim, char **hc_fpath);

// ft_hc_create_file helpers
char	*ft_hc_validate_path(char **hc_fpath, int *hc_count_int);
int		ft_hc_init_file(char **hc_fpath);

// errors
void	*ft_hc_syscall_error(char *message);
void	ft_hc_unlink_and_free(void *content);
void	*ft_hc_cap_error(char *message, char **hc_fpath, t_list **hdocs);

/*
Esta função serve para capturar input do usuário quando o operador << é usado.
Ela recebe como argumentos a string delim e o pointer para a o head da
linked list hdocs.

O valor de retorno hc_fpath serve apenas para indicar o sucesso da operação.
Se houver qualquer erro na captura, o valor de retorno é NULL. Em caso de
erro, toda a memória até então alocada e o prompt devolvido para o usuário.
*/

char	*ft_hc_capture(char *hc_delim, t_list **hdocs)
{
	static int	hc_count_int;
	t_heredoc	*hc_node;
	char		*hc_fpath;

		/*	assigning NULL porque eu vou passar esse pointer como argumento
		para uma função que vai preenchê-lo, e preciso inicializá-lo antes. */ 
	hc_fpath = NULL;

		/* vou chechar se os argumentos são válidos porque eu não sou o 
		responsável por eles. então preciso validar. daqui para frente, eu não
		valido mais os pointers e fds nas funções, e sim antes de enviá-los. */
	if (!hc_delim)
		return (ft_hc_cap_error("hc_capture: invalid args\n", &hc_fpath, hdocs));

		/* primeiro vamos criar a string do file path onde vamos criar o
		arquivo onde vamos salvar os comandos digitados no heredoc. */
	if (!ft_hc_create_file(&hc_count_int, &hc_fpath))
		return (ft_hc_cap_error("hc_capture: path error\n", &hc_fpath, hdocs));

		/* depois iniciamos a captura dos de cada comando em um loop */
	if (!ft_hc_input_loop(hc_delim, &hc_fpath))
		return (ft_hc_cap_error("hc_capture: loop error\n", &hc_fpath, hdocs));

		/* Sucesso? Então vamos criar um node de t_heredocs para armazenar
		o caminho do arquivo que vamos abrir na fase de execução. A cada vez
		que a ft_hc_capture é chamada, um node é acrescentado na lista. 
		Quando terminarmos de executar todos os comandos, precisamos limpar
		a lista de heredos, assim como a ft_hc_cap_error já faz em caso
		de erro.  */
	hc_node = malloc(sizeof(t_heredoc));
	if (!hc_node)
		return (ft_hc_cap_error("node malloc error\n", &hc_fpath, hdocs));
	hc_node->heredoc_file = hc_fpath;

		/* Chamamos a função da libft para incluir um node no fim da lista. 
		Para usarmos a função da libft, a gente precisa fazer typecasting
		toda hora que vamos mandar manda um node para lista ou ler o 
		content de um node que pegamos da lista (ver como isso é feito
		também na ft_hc_unlink_and_free) */
	ft_lstadd_back(hdocs, (t_list *)hc_node);

		/* Incrementamos o static count. Quando a ft_hc_capture for chamada
		de novo, o contador não zero. Depois preciso dar um jeito de
		zerar esse contador depois da fase de execução - ou vou precisar
		retirar minha verificação de maxint dos erros... */
	hc_count_int++;
	if (hc_count_int == INT_MAX)
		return (ft_hc_cap_error("node limit error\n", &hc_fpath, hdocs));
	
		/* Não vamos usar esse valor de retorno. Na fase de execução vamos
		acessar direto a t_list **hdocs. Mas o valor de retorno está aí
		para a que a função caller tenha como conferir o sucesso do heredoc.
		Eu fazia muita função void, mas isso é ruim para verificar erros.*/ 
	return (hc_fpath);
}







/******************************************************************************* 

Cria o arquivo temporário para receber o heredoc.

*******************************************************************************/

char	*ft_hc_create_file(int *hc_count_int, char **hc_fpath)
{
	char	*hc_count_str;

	hc_count_str = ft_itoa(*hc_count_int);
	if (!hc_count_str)
		return (ft_hc_syscall_error("hc_create_file - itoa error"));

	*hc_fpath = ft_strjoin("temp_heredocs/ms_temp_heredoc_", hc_count_str);
	free(hc_count_str);
	if (!(*hc_fpath))
		return (ft_hc_syscall_error("hc_create_file - strjoin error"));

	if (!ft_hc_validate_path(hc_fpath, hc_count_int))
		return (NULL);

	if (ft_hc_init_file(hc_fpath) == -1)
		return (NULL);

	return (*hc_fpath);
}



/******************************************************************************* 

O loop de captura para cada linha do readline.

*******************************************************************************/

char	*ft_hc_input_loop(char *hc_delim, char **hc_fpath)
{
	int		hc_temp_file;
	char	*hc_input;

	while(1)
	{
		hc_input = readline("> ");
		if (!hc_input)
			return (ft_hc_syscall_error("heredoc readline error"));

		if (ft_strcmp(hc_delim, hc_input) == 0)
			break ;

		hc_temp_file = open(*hc_fpath, O_WRONLY | O_APPEND, 0644);
		if (hc_temp_file == -1)
		{
			ft_free_and_null(hc_input);
			return (ft_hc_syscall_error("heredoc open error"));
		}
		ft_dprintf(hc_temp_file, "%s\n", hc_input);
		close(hc_temp_file);
		ft_free_and_null(hc_input);
	}
	ft_free_and_null(hc_input);
	return (*hc_fpath);
}



/******************************************************************************* 

Helpers da create file.

*******************************************************************************/

char	*ft_hc_validate_path(char **hc_fpath, int *hc_count_int)
{
	char		*hc_count_str;
	char		*hc_temp;

	while (1)
	{
		/* se o arquivo não existe, ou se existe com permissão de gravação,
		o loop encerra e a função retorna hc_path como sinal de sucesso. */
		if (!access(*hc_fpath, F_OK) || access(*hc_fpath, W_OK))
			break ;
		
		/* se o arquivo existe e não tem permissão de gravação, começa um
		loop para redefinir o nome do arquivo até que seja possível. */		
		else
		{
			(*hc_count_int)++;
			hc_count_str = ft_itoa(*hc_count_int);
			if (!hc_count_str)
				return (ft_hc_syscall_error("itoa error"));
			hc_temp = *hc_fpath;
			*hc_fpath = ft_strjoin("temp_heredocs/ms_temp_heredoc_", hc_count_str);        //pick another folder later.
			ft_free_and_null(hc_count_str);
			if (!(*hc_fpath))
			{
				ft_free_and_null(hc_temp);
				return (ft_hc_syscall_error("strjoin error"));
			}
			ft_free_and_null(hc_temp);
		}
	}
	return (*hc_fpath);
}


int	ft_hc_init_file(char **hc_fpath)
{
	int		hc_open_file;

	hc_open_file = open(*hc_fpath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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

void	*ft_hc_syscall_error(char *message)
{
	ft_dprintf(STDERR_FILENO, "%s: %s [%i]\n", message, strerror(errno), errno);
	return (NULL);
}

void	ft_hc_unlink_and_free(void *content)
{
	t_heredoc	*hc_node;

	hc_node = (t_heredoc *)content;
	
	if (hc_node->heredoc_file)
	{
		if (access(hc_node->heredoc_file, F_OK) == 0)
			unlink(hc_node->heredoc_file);
		ft_free_and_null(hc_node->heredoc_file);
	}
	content = NULL;
	return ;
}

void	*ft_hc_cap_error(char *message, char **hc_fpath, t_list **hdocs)
{
	ft_dprintf(STDERR_FILENO, "%s", message);

	if (access(*hc_fpath, F_OK))
		unlink(*hc_fpath);

	ft_lstclear(hdocs, ft_hc_unlink_and_free);

	if (*hc_fpath)
	{
		free(*hc_fpath);
		*hc_fpath = NULL;
	}

	return (NULL);
}
