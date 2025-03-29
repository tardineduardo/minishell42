/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 20:52:35 by eduribei          #+#    #+#             */
/*   Updated: 2025/03/08 16:22:50 by eduribei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_mem	*mem;

	(void)argc;
	(void)argv;
	mem = NULL;
	ft_init_minishell_memory(&mem, envp);

	while (1)
	{
		ft_readline(&(mem->capture), &(mem->tokenize), &(mem->heredoc));
		if (!mem->capture->line)
			continue ;
		add_history(mem->capture->line);
		//ft_tokenize(mem->capture->line, &mem->tokenize);
		ft_execute(mem->capture->line, &mem);
		ft_clean_mem_loop(&mem);
	}

	ft_clear_mem_and_exit(&mem);
	return (0);
}
void ft_clean_mem_loop(t_mem **mem)
{
	if ((*mem)->tokenize->toklst)
		ft_lstclear(&(*mem)->tokenize->toklst, ft_del_token_node);
	if ((*mem)->heredoc->delim)
		ft_free_and_null((void *)&(*mem)->heredoc->delim);
	if ((*mem)->capture->line)
		ft_free_and_null((void *)&(*mem)->capture->line);


	if ((*mem)->tokenize->str)
		ft_free_and_null((void *)&(*mem)->tokenize->str);

	return ;
}
