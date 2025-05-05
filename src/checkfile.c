#include "../include/expand.h"
#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/parsing.h"

#include <sys/stat.h> // stat

#define EXIS    0x01	// 0000 0000 0001 0000
#define READ    0x02	// 0000 0000 0010 0000
#define WRIT    0x04	// 0000 0000 0100 0000
#define EXEC    0x08	// 0000 0000 1000 0000
#define ISFLE   0x10	// 0000 0001 0000 0000
#define ISDIR   0x20	// 0000 0010 0000 0000

typedef struct s_error
{
	int		errnum;
	char	*errmsg;
} t_error;


bool	check(char *abspath, u_int16_t flags, t_error **error)
{
	struct stat	statbuf;

	if (abspath[0] != '/')
		return (false); //maybe print error?
	if (flags & EXIS && access(abs, F_OK))
		return (return_error(abs, error));
	if (flags & READ && access(abs, R_OK))
		return (return_error(abs, error));
	if (flags & WRIT && access(abs, W_OK))
		return (return_error(abs, error));
	if (flags & EXEC && access(abs, X_OK))
		return (return_error(abs, error));
	if (stat(abspath, &statbuf))
		return (return_error(abs, error));
	if (flags & ISFLE && !S_ISREG(statbuf.st_mode))
		return (return_error(abs, error));
	if (flags & ISDIR && !S_ISDIR(statbuf.st_mode))
		return (return_error(abs, error));
	return (abs);

}


bool	*return_error(char *abs, t_error **error)
{
	if (!error || !*error)
		return (false);
	(*error)->errnum = errno;
	(*error)->errmsg = strerror(errno);
	return (false);
}


char *get_absolute_path_file(char *file, t_mem *mem)
{
	if (!file || !mem)
		return (NULL);

	if (strncmp(file, "/", 1))
		return (ft_strdup(file));
	if (strncmp(file, "./", 2))
		return (get_working_dir(file, mem));
	if (strncmp(file, "../", 3))
		return (get_relative_path);
	return (NULL);
}

char *get_absolute_path_bin(char *file, u_int8_t mode, t_mem *mem)
{

}

char *get_absolute_path_folder(char *file, u_int8_t mode, t_mem *mem)
{

}




//malloc
char *get_working_dir(char *file, t_mem *mem)
{
	char		*path;
	t_list		*trav;
	t_list		*envlist;
	t_env_node	*envnode;

	envlist = ((t_env_mem *)mem->environs)->envlist;

	trav = envlist;
	while(trav)
	{
		envnode = (t_env_node *)trav->content;
		if(ft_strcmp(envnode->variable, "PWD") == 0)
			break ;
		trav = trav->next;
	}
	if (!trav)
		return (NULL);
	path = ft_concatenate_var(3, envnode->value, "/", file);
	return (path);
}





char *get_relative_path(char *file, t_mem *mem)
{
	char		*path;
	char		**steps;
	t_list		*trav;
	t_list		*envlist;
	t_env_node	*envnode;

	envlist = ((t_env_mem *)mem->environs)->envlist;

	trav = envlist;
	while(trav)
	{
		envnode = (t_env_node *)trav->content;
		if(ft_strcmp(envnode->variable, "PWD") == 0)
			break ;
		trav = trav->next;
	}
	if (!trav)
		return (NULL);

	steps = ft_split_char(file, '/');
	int max = ft_split_count(file, '/'); 
	int a = 0; 

	while(a < max)
	{
		if(ft_strcmp(steps[a], "..") == 0)
		{}	//remove last folder from pwd
		else if (ft_strcmp(steps[a], ".") != 0)
		{}	//apend /folder to pwd - check it is a folder
		a++;
	}
}

























static char	*get_absolute_path(t_cmd *cmd, char *argv[])
{
	char	*path;
	char	**commands;

	commands = ft_split_space(argv[cmd->cmd_index]);
	if (!commands)
		return (NULL);
	path = ft_strdup(commands[0]);
	if (!path)
		return (NULL);
	ft_free_str_array(commands);
	return (path);
}


static char	*get_relative_path(char *envp[], t_cmd *cmd)
{
	int		a;
	char	*pwd;
	char	*path;
	char	*ptr;

	a = 0;
	while (envp[a] != NULL)
	{
		pwd = ft_strnstr(envp[a++], "PWD=", 4);
		if (pwd)
		{
			pwd = ft_strdup(pwd + 4);
			break ;
		}
	}	
	if (!pwd)
		return (NULL);
	if (cmd->path_is_parent_dir)
	{
		ptr = ft_strrchr(pwd, '/');
		*ptr++ = '\0';
	}
	path = ft_concatenate(pwd, "/", cmd->cmd[0]);
	free(pwd);
	return (path);
}


static char	*ft_get_abs_env_path(char **path_split, char *path, char *cmd)
{
	int	i;

	i = 0;
	while (path_split[i] != NULL)
	{
		path = ft_concatenate(path_split[i], "/", cmd);
		if (path == NULL)
			return (NULL);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

static char	*get_environment_path(char *envp[], char *cmd0)
{
	int		a;
	char	*path;
	char	**path_split;

	a = 0;
	while (envp[a] != NULL)
	{
		path = ft_strnstr(envp[a], "PATH=", 5);
		if (path)
		{
			path_split = ft_split_char((path + 5), ':');
			if (path_split == NULL)
				return (NULL);
			path = ft_get_abs_env_path(path_split, path, cmd0);
			ft_free_str_array(path_split);
			if (!path)
				return (NULL);
			return (path);
		}
		a++;
	}
	return (NULL);
}

char	*parse_path(char *envp[], t_cmd *cmd, char *argv[])
{
	if (!cmd->cmd)
		return (NULL);
	if (cmd->path_is_environment)
		return (parse_environment_path(envp, cmd->cmd[0]));
	else if (cmd->path_is_absolute)
		return (parse_absolute_path(cmd, argv));
	else if (cmd->path_is_curr_dir || cmd->path_is_parent_dir)
		return (parse_relative_path(envp, cmd));
	return (NULL);
}
