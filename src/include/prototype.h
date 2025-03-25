#ifndef PROTOTYPE_H
# define PROTOTYPE_H
/*
	TODO remmember to update the real one and delete this dir and file
*/
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include "../../include/minishell.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_input
{
	t_list	*input_lst;
	char	*name;
	int		fd;
}	t_input;

typedef struct s_output
{
	t_list	*output_lst;
	char	*name;
	int		fd;
}	t_output;

typedef struct 	s_env_node
{
	char	*variable;
	char	*value;
	bool	readonly;
	bool	block_unset;
}	t_env_node;

typedef struct s_env_mem
{
	t_list		*envlist;
	t_env_node	*new_node;
	char		**result;
}	t_env_mem;

typedef struct s_cmd_node
{
	t_list	*cmd_lst;
	char	**cmd_arr;
	t_list	*input_lst;
	t_list	*output_lst;
	t_list	*heredoc_lst;
	t_list	*o_concat;
	int		err;
}			t_cmd_node;

int multiple_pipes(t_list **cmd_lst, char **ms_env);

#endif