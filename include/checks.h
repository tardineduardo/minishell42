# include "minishell.h"  
# include "../libs/libft/libft.h"

typedef struct s_error
{
	int		errnum;
	char	*errmsg;
} t_error;

char	*get_absolute_path(char *file, t_mem **mem);
char	*get_working_dir(char *file, t_mem *mem);
char	*get_relative_path(char *relative_path, t_mem **mem);
char	*remove_last_folder_from_cwd(char **cwd);
void	*append_to_cwd(char **cwd, char *append);
char	*get_cwd(t_mem *mem);
bool	*return_error(char *abs, t_error **error);
