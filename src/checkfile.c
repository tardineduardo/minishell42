#include "../include/expand.h"
#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/parsing.h"
#include "../include/checkfile.h"


#include <sys/stat.h> // stat

#define EXIS    0x01	// 0000 0000 0001 0000
#define READ    0x02	// 0000 0000 0010 0000
#define WRIT    0x04	// 0000 0000 0100 0000
#define EXEC    0x08	// 0000 0000 1000 0000
#define ISFLE   0x10	// 0000 0001 0000 0000
#define ISDIR   0x20	// 0000 0010 0000 0000




bool	check(char *abspath, u_int16_t flags, t_error **error)
{
	struct stat	statbuf;

	if (abspath[0] != '/')
		return (false); //maybe print error?
	if (flags & EXIS && access(abspath, F_OK))
		return (return_error(abspath, error));
	if (flags & READ && access(abspath, R_OK))
		return (return_error(abspath, error));
	if (flags & WRIT && access(abspath, W_OK))
		return (return_error(abspath, error));
	if (flags & EXEC && access(abspath, X_OK))
		return (return_error(abspath, error));
	if (stat(abspath, &statbuf))
		return (return_error(abspath, error));
	if (flags & ISFLE && !S_ISREG(statbuf.st_mode))
		return (return_error(abspath, error));
	if (flags & ISDIR && !S_ISDIR(statbuf.st_mode))
		return (return_error(abspath, error));
	return (abspath);

}


bool	*return_error(char *abs, t_error **error)
{
	(void)abs;
	if (!error || !*error)
		return (false);
	(*error)->errnum = errno;
	(*error)->errmsg = strerror(errno);
	return (false);
}


char *get_absolute_path_file(char *file, t_mem **mem)
{
	if (!file || !mem)
		return (NULL);

	if (strncmp(file, "/", 1))
		return (ft_strdup(file));
	if (strncmp(file, "../", 3))
		return (get_relative_path(file, mem));
	return (NULL);
}

// char *get_absolute_path_bin(char *file, u_int8_t mode, t_mem *mem)
// {

// }

// char *get_absolute_path_folder(char *file, u_int8_t mode, t_mem *mem)
// {

// }




char *get_relative_path(char *relative_path, t_mem **mem)
{
	char	*pwd;
	char	**split;
	int		count;
	int		a;
	
	pwd = get_pwd(*mem);
	count = ft_split_count(relative_path, '/');
	split = ft_split_char(relative_path, '/');
	if (!split || !pwd)
		return (NULL);

	
	a = 0;
	while(a < count)
	{
		if(ft_strcmp(split[a], "..") == 0)
			remove_last_folder_from_pwd(&pwd);
		else if (ft_strcmp(split[a], ".") != 0)
			append_to_pwd(&pwd, split[a]);
		a++;
	}
	return(pwd);
}

char	*remove_last_folder_from_pwd(char **pwd)
{
	char	*last_slash;
	int		new_len;
	char	*temp;

	last_slash = ft_strrchr(*pwd, '/');

	new_len = ft_strlen(*pwd) - ft_strlen(last_slash);

	temp = *pwd;
	*pwd = ft_substr(*pwd, 0, new_len);
	if (!*pwd)
	{
		free(temp);
		return (NULL);
	}
	free(temp);
	return(*pwd); //valor de retorno apenas para indicar sucesso
}


void	*append_to_pwd(char **pwd, char *append)
{
	char	*temp;

	temp = *pwd;
	*pwd = ft_strjoin(*pwd, append);
	if (!*pwd)
	{
		free(temp);
		return (NULL);
	}
	free(temp);
	return(*pwd);
}




char *get_pwd(t_mem *mem)
{
	char		*value;
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

	value = ft_strdup(envnode->value);
	if (!value)
		return (NULL);

	return (value);	
}




















// static char	*get_absolute_path(t_cmd *cmd, char *argv[])
// {
// 	char	*path;
// 	char	**commands;

// 	commands = ft_split_space(argv[cmd->cmd_index]);
// 	if (!commands)
// 		return (NULL);
// 	path = ft_strdup(commands[0]);
// 	if (!path)
// 		return (NULL);
// 	ft_free_str_array(commands);
// 	return (path);
// }


// static char	*get_relative_path(char *envp[], t_cmd *cmd)
// {
// 	int		a;
// 	char	*pwd;
// 	char	*path;
// 	char	*ptr;

// 	a = 0;
// 	while (envp[a] != NULL)
// 	{
// 		pwd = ft_strnstr(envp[a++], "PWD=", 4);
// 		if (pwd)
// 		{
// 			pwd = ft_strdup(pwd + 4);
// 			break ;
// 		}
// 	}	
// 	if (!pwd)
// 		return (NULL);
// 	if (cmd->path_is_parent_dir)
// 	{
// 		ptr = ft_strrchr(pwd, '/');
// 		*ptr++ = '\0';
// 	}
// 	path = ft_concatenate(pwd, "/", cmd->cmd[0]);
// 	free(pwd);
// 	return (path);
// }


// static char	*ft_get_abs_env_path(char **path_split, char *path, char *cmd)
// {
// 	int	i;

// 	i = 0;
// 	while (path_split[i] != NULL)
// 	{
// 		path = ft_concatenate(path_split[i], "/", cmd);
// 		if (path == NULL)
// 			return (NULL);
// 		if (access(path, F_OK) == 0)
// 			return (path);
// 		free(path);
// 		i++;
// 	}
// 	return (NULL);
// }

// static char	*get_environment_path(char *envp[], char *cmd0)
// {
// 	int		a;
// 	char	*path;
// 	char	**path_split;

// 	a = 0;
// 	while (envp[a] != NULL)
// 	{
// 		path = ft_strnstr(envp[a], "PATH=", 5);
// 		if (path)
// 		{
// 			path_split = ft_split_char((path + 5), ':');
// 			if (path_split == NULL)
// 				return (NULL);
// 			path = ft_get_abs_env_path(path_split, path, cmd0);
// 			ft_free_str_array(path_split);
// 			if (!path)
// 				return (NULL);
// 			return (path);
// 		}
// 		a++;
// 	}
// 	return (NULL);
// }

// char	*parse_path(char *envp[], t_cmd *cmd, char *argv[])
// {
// 	if (!cmd->cmd)
// 		return (NULL);
// 	if (cmd->path_is_environment)
// 		return (parse_environment_path(envp, cmd->cmd[0]));
// 	else if (cmd->path_is_absolute)
// 		return (parse_absolute_path(cmd, argv));
// 	else if (cmd->path_is_curr_dir || cmd->path_is_parent_dir)
// 		return (parse_relative_path(envp, cmd));
// 	return (NULL);
// }
