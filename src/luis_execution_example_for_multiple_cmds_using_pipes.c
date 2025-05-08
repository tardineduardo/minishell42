/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luis_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luide-ca <luide-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:28:54 by luide-ca          #+#    #+#             */
/*   Updated: 2025/04/23 11:57:14 by luide-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	exec_external_cmd(t_list **ms_env, t_cmd_node *cmd)
// {
// 	char **ms_env_arr;
// 	char **cmd_arr;
// 	pid_t	cpid;

// 	cmd_arr = cmd->cmd_arr;
// 	cmd_arr = update_cmd_arr(ms_env, cmd_arr);
// 	ms_env_arr = ft_ms_env_arr(ms_env);
// 	cpid = fork();
// 	if (cpid == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (cpid == 0)
// 	{
// 		if (execve(cmd->cmd_arr[0], cmd->cmd_arr, ms_env_arr) == -1)
// 		{
// 			perror("execve");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// }

// int	exec_cmd_WIP(t_list **ms_env, t_cmd_node *cmd)
// {
// 	char **cmd_arr;

// 	if (!ms_env || !cmd)
// 	{
// 		perror("exec cmd");
// 		exit(EXIT_FAILURE);
// 	}
// 	cmd_arr = cmd->cmd_arr;
// 	if (is_built_in(cmd_arr))
// 		exec_built_in(ms_env, cmd_arr);
// 	else
// 		exec_external_cmd(ms_env, cmd);
// 	return (0);
// }

int	exec_pipe(t_list **ms_env, t_list **tokens, int num_cmds)
{
	t_list		*cur;
	t_org_tok	*org_token;
	char		**cmd_arr1;
	char		**cmd_arr2;
	char		**cmd_arr3;
	char		**ms_env_arr1;
	char		**ms_env_arr2;
	char		**ms_env_arr3;
	int			pipefd1[2];
	int			pipefd2[2];
	int			status1;
	int			status2;
	int			status3;
	pid_t		cpid1;
	pid_t		cpid2;
	pid_t		cpid3;

	(void)num_cmds;
	cur = *tokens;
	if (pipe(pipefd1) == -1)
	{
		perror("pipe call");
		exit(EXIT_FAILURE);
	}
	cpid1 = fork();
	if (cpid1 == -1)
	{
		perror("fork 1 call");
		exit(EXIT_FAILURE);
	}
	if (cpid1 == 0)
	{
		org_token = cur->content;
		cmd_arr1 = org_token->cmd_node->cmd_arr;
		cmd_arr1 = update_cmd_arr(ms_env, cmd_arr1);
		ms_env_arr1 = ft_ms_env_arr(ms_env);
		dup2(pipefd1[1], STDOUT_FILENO);
		close(pipefd1[1]);
		close(pipefd1[0]);
		if (execve(cmd_arr1[0], cmd_arr1, ms_env_arr1) == -1)
		{
			fprintf(stderr, "%s: command not found\n", cmd_arr1[0]);
			exit(EXIT_FAILURE);
		}
	}
	cur = cur->next;
	cur = cur->next;
	if (pipe(pipefd2) == -1)
	{
		perror("pipe call");
		exit(EXIT_FAILURE);
	}
	cpid2 = fork();
	if (cpid2 == -1)
	{
		perror("fork 2 call");
		exit(EXIT_FAILURE);
	}
	if (cpid2 == 0)
	{
		org_token = cur->content;
		cmd_arr2 = org_token->cmd_node->cmd_arr;
		cmd_arr2 = update_cmd_arr(ms_env, cmd_arr2);
		ms_env_arr2 = ft_ms_env_arr(ms_env);
		dup2(pipefd1[0], STDIN_FILENO);
		close(pipefd1[0]);
		dup2(pipefd2[1], STDOUT_FILENO);
		close(pipefd2[1]);
		close(pipefd1[1]);
		close(pipefd2[0]);
		if (execve(cmd_arr2[0], cmd_arr2, ms_env_arr2) == -1)
		{
			fprintf(stderr, "%s: command not found\n", cmd_arr2[0]);
			exit(EXIT_FAILURE);
		}
	}
	cur = cur->next;
	cur = cur->next;
	cpid3 = fork();
	if (cpid3 == -1)
	{
		perror("fork 3 call");
		exit(EXIT_FAILURE);
	}
	if (cpid3 == 0)
	{
		org_token = cur->content;
		cmd_arr3 = org_token->cmd_node->cmd_arr;
		cmd_arr3 = update_cmd_arr(ms_env, cmd_arr3);
		ms_env_arr3 = ft_ms_env_arr(ms_env);
		dup2(pipefd2[0], STDIN_FILENO);
		close(pipefd2[0]);
		close(pipefd2[1]);
		close(pipefd1[0]);
		close(pipefd1[1]);
		if (execve(cmd_arr3[0], cmd_arr3, ms_env_arr3) == -1)
		{
			fprintf(stderr, "%s: command not found\n", cmd_arr3[0]);
			exit(EXIT_FAILURE);
		}
	}
	close(pipefd1[0]);
	close(pipefd1[1]);
	close(pipefd2[0]);
	close(pipefd2[1]);
	waitpid(cpid1, &status1, 0);
	waitpid(cpid2, &status2, 0);
	waitpid(cpid3, &status3, 0);
	printf("\ncp1: %d, cp2: %d, cpd3: %d\n", status1, status2, status3);
	return (0);
}

