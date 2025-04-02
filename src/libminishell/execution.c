/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:33:18 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/02 15:19:09 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/prototype.h"

void	exec_command(t_list **ms_env, t_cmd_node	*cur_cmd)
{
	char	**cmd_arr;

	cmd_arr = cur_cmd->cmd_arr;
	if (!cur_cmd || !ms_env)
	{
		perror("cmd or ms_env: cmd executor");
		exit(EXIT_FAILURE);
	}
	if (is_built_in(cmd_arr))
		exec_built_in(ms_env, cmd_arr);
	else
		exec_external_cmd(ms_env, cur_cmd);
}

int	exec_pipeline(t_list **ms_env, t_list **cmd)
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
			// if (!is_built_in(&cur_cmd->cmd_arr[0]))// need to implement a if where only external command are acceptable to redirect of input
			// 	fd_input_redir(&cur_cmd->input_lst);
			if (cmds_counter > 1)
				pipe_fd_control(i, cmds_counter, &cur_cmd->input_lst, &cur_cmd->output_lst, pipefd[0], pipefd[1], fd_in);
			// if (!is_built_in(&cur_cmd->cmd_arr[0]))
			// 	fd_output_redir(&cur_cmd->output_lst);
			exec_command(ms_env, cur_cmd);
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
	while (waitpid(-1, &status, 0) > 0)
	if (WIFEXITED(status))
    	return (WEXITSTATUS(status));
	return (-1);
}