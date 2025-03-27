/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:33:18 by luide-ca          #+#    #+#             */
/*   Updated: 2025/03/27 18:41:43 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/prototype.h"

void	ft_command_executor(t_list **ms_env, t_cmd_node	*cur_cmd)
{
	if (!cur_cmd || !ms_env)
	{
		perror("cmd or ms_env: cmd executor");
		exit(EXIT_FAILURE);
	}
	if (is_built_in(cur_cmd))
		exec_built_in(ms_env, cur_cmd);
	else
		exec_external_cmd(ms_env, cur_cmd);
}

int	ft_prompt_execution(t_list **ms_env, t_list **cmd)
{
	pid_t	cpid;
	int		*pipefd;
	int		status;
	int		cmds_counter;
	int		i;
	int		fd_in;
	t_cmd_node	*cur_cmd;
	t_list		*cur_node;

	fd_in = 0;
	if (!cmd || !ms_env)
	{
		perror("cmd or ms_env: execution");
		exit(EXIT_FAILURE);
	}
	cmds_counter = ft_lstsize(*cmd);
	cur_node = *cmd;
	i = 0;
	while (i < cmds_counter)
	{
		cur_cmd = cur_node->content;
		if (i < cmds_counter - 1 && cmds_counter > 1)
			pipefd = ft_pipe_control();
		cpid = ft_fork_control();
		if (cpid == 0)
		{
			fd_input_redir(&cur_cmd->input_lst);
			if (cmds_counter > 1)
				pipefd_control(i, cmds_counter, pipefd[0], pipefd[1], fd_in);
			fd_output_redir(&cur_cmd->output_lst);
			ft_command_executor(ms_env, cur_cmd);
		}
		if (i > 0) // Close previous read end
			close(fd_in);
		fd_in = pipefd[0]; // Update for next iteration
		if (i < cmds_counter - 1)
			close(pipefd[1]); // Close write end after use
		cur_node = cur_node->next;
		i++;
	}
	// make this part below a separate function, now only the last status is returned.
	while (waitpid(cpid, &status, 0) > 0);
	if (WIFEXITED(status))
    	return (WEXITSTATUS(status));
	return (-1);
}