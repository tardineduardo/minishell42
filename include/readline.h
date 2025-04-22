#ifndef READLINE_H
# define READLINE_H

# include "minishell.h"  
# include "heredoc.h"
# include "expand.h"
# include "tokenize.h"
# include "environs.h"

# include "../libs/libft/libft.h"



typedef struct	s_rdl_mem
{
	char		*line;
	char		*trim;
	char		*temp;
	char		*append;
}	t_rdl_mem;

void	*ft_readline(t_mem **mem);
bool	ft_line_is_incomplete(char *s);
char	*ft_rdl_input_loop(t_mem **mem);


#endif