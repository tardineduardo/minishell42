#ifndef READLINE_H
# define READLINE_H

# include "minishell.h" 
# include "heredoc.h"
# include "expand.h"
# include "tokenize.h"
# include "environs.h"

# include "../libs/libft/libft.h"

typedef struct s_mem	t_mem;

typedef struct s_rdl_mem
{
	char		*line;
	char		*trim;
	char		*temp;
	char		*append;
}	t_rdl_mem;

int		ft_readline(t_mem **mem);
bool	ft_line_is_incomplete(char *s);
char	*ft_rdl_input_loop(t_mem **mem);
void	*ft_init_rdl_memory(t_mem **mem);
void	ft_clear_rdl_mem(t_rdl_mem **rdl);

#endif
