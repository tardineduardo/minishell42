#include "../include/expand.h"
#include "../include/minishell.h"
#include "../include/heredoc.h"
#include "../include/tokenize.h"
#include "../include/parsing.h"
#include "../include/checks.h"

#include <sys/stat.h> // stat

#define EXIS    0x01	// 0000 0000 0001 0000
#define READ    0x02	// 0000 0000 0010 0000
#define WRIT    0x04	// 0000 0000 0100 0000
#define EXEC    0x08	// 0000 0000 1000 0000
#define ISFLE   0x10	// 0000 0001 0000 0000
#define ISDIR   0x20	// 0000 0010 0000 0000


// Check file properties based on flags (existence, permissions, type)
bool	validate_file_(char *abspath, u_int16_t flags, t_error **error)
{
	struct stat	statbuf;

	if (abspath[0] != '/')
		return (false); // maybe print error?
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

// Populate error struct with errno and message, return false
bool	*return_error(char *abs, t_error **error)
{
	(void)abs;
	if (!error || !*error)
		return (false);
	(*error)->errnum = errno;
	(*error)->errmsg = strerror(errno);
	return (false);
}

// Return absolute path for a file; supports absolute and relative paths
char *get_absolute_path(char *execfile, t_mem **mem)
{
	if (!execfile || !mem)
		return (NULL);
	if (ft_strncmp(execfile, "/", 1) == 0)
		return (get_abs_path(execfile));
	if (ft_strncmp(execfile, "../", 3) == 0 || ft_strncmp(execfile, "./", 2) == 0)
		return (get_relative_path(execfile, mem));
	else
		return (get_env_path(execfile, mem));
	return (NULL);
}



char *get_abs_path(char *execfile)
{
	char *abs;

	abs = ft_strdup(execfile);
	if (!abs)
		return (NULL);
	if (access(abs, F_OK) == 0)
		return (abs);
	free(abs);
	return (NULL);
}

char *get_env_path(char *file, t_mem **mem)
{
	(void)file;
	(void)mem;
	return (NULL);
}


// Build full absolute path from relative path (e.g., "../dir/file")
char *get_relative_path(char *relative_path, t_mem **mem)
{
	char	*cwd;
	char	**split;
	int		count;
	int		a;

	cwd = get_cwd(*mem);
	count = ft_split_count(relative_path, '/');
	split = ft_split_char(relative_path, '/');
	if (!split || !cwd)
		return (NULL);
	a = 0;
	while (a < count)
	{
		if (ft_strcmp(split[a], "..") == 0 && cwd[0] != '\0')
			remove_last_folder_from_cwd(&cwd);
		else if (ft_strcmp(split[a], ".") != 0 && ft_strcmp(split[a], "..") != 0)
			append_to_cwd(&cwd, split[a]);
		a++;
	}
	ft_free_str_array(split);
	return (cwd);
}

// Remove the last folder from a path (modifies cwd in place)
char	*remove_last_folder_from_cwd(char **cwd)
{
	char	*last_slash;
	int		new_len;
	char	*temp;

	last_slash = ft_strrchr(*cwd, '/');
	new_len = ft_strlen(*cwd) - ft_strlen(last_slash);
	temp = *cwd;
	*cwd = ft_substr(*cwd, 0, new_len);
	if (!*cwd)
	{
		free(temp);
		return (NULL);
	}
	free(temp);
	return (*cwd); // return for success indication
}

// Append a new folder to the path (modifies cwd in place)
void	*append_to_cwd(char **cwd, char *append)
{
	char	*temp;

	temp = *cwd;
	*cwd = ft_concatenate(*cwd, "/", append);
	if (!*cwd)
	{
		free(temp);
		return (NULL);
	}
	free(temp);
	return (*cwd);
}

// Get the current working directory ("PWD") from environment list
char *get_cwd(t_mem *mem)
{
	char		*value;
	t_list		*trav;
	t_list		*envlist;
	t_env_node	*envnode;

	envlist = ((t_env_mem *)mem->environs)->envlist;

	trav = envlist;
	while (trav)
	{
		envnode = (t_env_node *)trav->content;
		if (ft_strcmp(envnode->variable, "PWD") == 0)
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