# include "minishell.h"  
# include "../libs/libft/libft.h"

typedef struct s_error
{
	int		errnum;
	char	*errmsg;
} t_error;

char	*get_absolute_path_file(char *file, t_mem **mem);
char	*get_working_dir(char *file, t_mem *mem);
char	*get_relative_path(char *relative_path, t_mem **mem);
char	*remove_last_folder_from_pwd(char **pwd);
void	*append_to_pwd(char **pwd, char *append);
char	*get_pwd(t_mem *mem);
bool	*return_error(char *abs, t_error **error);
