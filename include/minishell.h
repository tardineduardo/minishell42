/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:30 by eduribei          #+#    #+#             */
/*   Updated: 2025/02/21 21:02:12 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../libs/libft/libft.h"

typedef struct s_heredoc
{
	char	*heredoc_file;
}			t_heredoc;

typedef struct s_mem
{
	//heredoc
	t_list		**heredocs;


	//capture_command




}	t_mem;

char *ft_capture_command(void);
char *ft_run_command(char *line);
