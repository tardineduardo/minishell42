/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduribei <eduribei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:41:58 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 12:50:28 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"
# include "expand.h"
# include "environs.h"

typedef enum e_delim
{
	EXPAND,
	QUOTED,
}	t_delim;

typedef enum e_exit
{
	ERROR = 0,
	BAD_SUBSTITUITION = 0,
	INIT,
	SUCCESS,
	VAR_FOUND,
	VAR_NOT_FOUND,
	EMPTY_VARLIST,
}	t_exit;

typedef struct s_exp_mem
{
	int		a;
	int		b;
	char	*raw;
	char	*new;
	char	*temp;
	bool	error;
	bool	braces;
	char	*value;
	t_list	*sortedvars;
	size_t	alloct;
	t_delim	hd_mode;
	t_mode	mode;
	t_exit	exit;
}	t_exp_mem;

char	*ft_expand(char **string, t_mode mode, t_mem **mem);
void	*ft_init_exp_memory(t_mem **mem);
void	ft_clear_exp_mem(t_exp_mem **exp);
void	ft_reset_parameters(t_mem **mem);

//steps
void	ft_update_quote_flag(char *s, t_quote *quote, int index);
bool	ft_skip_if_quote_changd(t_exp_mem **exp, t_quote *quote, t_quote *prev);
bool	ft_process_inside_sg_quotes(t_exp_mem **exp, t_quote quote);
bool	ft_process_inside_db_quote(t_exp_mem **exp, t_mem **mem, t_quote quote);
bool	ft_process_unquoted_sequence(t_exp_mem **exp, t_mem **mem);
bool	ft_handle_dollar_sign(t_exp_mem **exp, t_mem **mem);
bool	ft_handle_backslash(t_exp_mem **exp, t_mode mode, t_quote quote);
t_exit	ft_try_to_expand_variable(t_exp_mem **exp, t_mem **mem);

//findvar
t_exit	ft_get_variable_value(char *dollar, char **value, t_mem **mem);
t_exit	ft_insert_var_in_string(char *insert, size_t index, t_exp_mem **exp);

//validations
t_delim	ft_is_delim_normal_or_quoted(char *s);
bool	ft_is_char_escaped(char *string, int a);
bool	ft_is_closing_quote(char c, t_quote *quote);

//ops
void	ft_copy_char_and_increment(t_exp_mem **exp);
bool	ft_copy_char_copy_next_and_increment(t_exp_mem **exp);
bool	ft_skip_slash_copy_next_and_increment(t_exp_mem **exp);
void	ft_skip_char_no_copy(t_exp_mem **exp);
void	ft_copy_value_and_increment(t_exp_mem **exp);

#endif