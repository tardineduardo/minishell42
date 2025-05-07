/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:44:56 by luide-ca          #+#    #+#             */
/*   Updated: 2025/05/07 13:44:25 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int execute_command(t_list **ms_env, t_cmd_node *cur_cmd, t_mem **mem)
{
    char    **cmd_arr;
    char    **final_cmd_arr;
    int     res;
    int     size_arr;
    int     i;

    res = -1;
    i = 0;
    if (!cur_cmd || !ms_env)
    {
        ft_putstr_fd("cmd or ms_env: cmd executor: NULL pointer\n", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    cmd_arr = cur_cmd->cmd_arr;
    size_arr = ft_count_items(cmd_arr);
    final_cmd_arr = malloc(sizeof(char *) * (size_arr));
    if (!final_cmd_arr)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    while (cmd_arr[i] != NULL)
    {
        final_cmd_arr[i] = ft_strdup(ft_expand(&cur_cmd->cmd_arr[i], TOKEN, mem));
        if (!final_cmd_arr[i])
        {
            perror("ft_strdup");
            while (--i >= 0)
                free(final_cmd_arr[i]);
            free(final_cmd_arr);
            exit(EXIT_FAILURE);
        }
        free(cur_cmd->cmd_arr[i]);
        i++;
    }
    final_cmd_arr[i] = NULL;
    free(cur_cmd->cmd_arr);
    cur_cmd->cmd_arr = final_cmd_arr;

    if (is_built_in(cur_cmd->cmd_arr))
        res = exec_built_in(ms_env, cur_cmd->cmd_arr);
    else
        exec_external_cmd(ms_env, cur_cmd);  // Assuming you modify this to return a value
    return (res);
}

void execute_child_pipe_command(t_pipe_data *p, t_list **ms_env, t_cmd_node *cmd, t_mem **mem)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	p->child_pids[p->i] = pid;
	if (pid == 0)
	{
		signal_child_process();
		pipe_fd_control(p, cmd, p->pipefd);
		execute_command(ms_env, cmd, mem);
		exit(EXIT_SUCCESS);
	}
}
int	print_child_statuses(t_pipe_data *p, int *status)
{
	int sig;
	int	index;
	int	i;
	int	res;

	if (p == NULL)
		i = 1;
	else
		i = p->num_cmds;
	index = 0;
	res = -1;
	while (index < i)
	{
		if (WIFSIGNALED(status[index]))
		{
			sig = WTERMSIG(status[index]);
			if (sig == SIGQUIT)
				return(ft_printf("Quit (core dumped)\n"));
			if (sig == SIGINT)
				res = (sig + 128);
			return (res);
		}
		if (WIFEXITED(status[index]))
			res = WEXITSTATUS(status[index]);
		index++;
	}
	return (res);
}

int	exec_single_cmd(t_list **ms_env, t_cmd_node *cmd, t_mem **mem)
{
	pid_t	pid;
	int		status;
	char	**cmd_arr;
	int		res;

	res = -1;
	cmd_arr = cmd->cmd_arr;
	if (!is_built_in(cmd_arr))
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			signal_child_process();
			fd_input_redir(&cmd->input_lst);
			fd_output_redir(&cmd->output_lst);
			execute_command(ms_env, cmd, mem);
			exit(EXIT_SUCCESS);
		}
		signal_before_wait();
		waitpid(pid, &status, 0);
		signal_after_wait();
		res = print_child_statuses(NULL, &status);
	}
	else
		res = execute_command(ms_env, cmd, mem);
	return (res);
}

int	wait_for_all_children(t_pipe_data p)
{
	int	i;
	int	status;

	i = 0;
	if (p.i == p.num_cmds && p.prev_fd != 0)
		close(p.prev_fd);
	while (i < p.num_cmds)
	{
		if (p.child_pids[i] > 0)
		{
			waitpid(p.child_pids[i], &status, 0);
			p.status_arr[p.i] = status;
		}
		i++;
	}
	return(print_child_statuses(&p, p.status_arr));
}

int execute_pipeline(t_list **env, t_list **org_token, int num_cmds, t_mem **mem)
{
	t_pipe_data p;
	t_list *node;
	
	ft_bzero(p.child_pids, sizeof(pid_t) * num_cmds);
	ft_bzero(p.status_arr, sizeof(pid_t) * num_cmds);
	node = *org_token;
	p.prev_fd = 0;
	p.num_cmds = num_cmds;
	p.i = 0;
	while (node && p.i < num_cmds)
	{
		if (p.i < num_cmds - 1 && pipe(p.pipefd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		execute_child_pipe_command(&p, env, (t_cmd_node *)node->content, mem);
		if (p.i > 0)
			close(p.prev_fd);
		p.prev_fd = p.pipefd[0];
		close(p.pipefd[1]);
		p.i++;
		node = node->next;
	}
	return(wait_for_all_children(p));
}
