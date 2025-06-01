#ifndef HEREDOC_H
# define HEREDOC_H

# include "minishell.h" 
# include "tokenize.h"

typedef struct	s_mem t_mem;
typedef enum e_quote t_quote;

typedef struct s_hdc_mem
{
	char		*delim;
	char		*filepath;
	char		*looptemp;
	char		*loopinput;
	t_mode		mode;
}	t_hdc_mem;

void	*ft_init_hdc_memory(t_mem **mem);
void	ft_clear_hdc_mem(t_hdc_mem **hd);
void	ft_free_mem_in_heredoc_child(int fd, char *filepath, t_mem **mem);
char	*ft_heredoc(char *delimiter, t_mem **mem);
void	run_heredoc_child(char *filepath, char *delimiter, t_mem **mem);
char	*ft_hd_create_file(int *hd_count_int, char **filepath);
char	*ft_hd_input_loop(t_list **envlist, t_mem **mem);
void	*ft_hd_write_to_file(int hd_loop_count, t_mem **mem);
char	*ft_hd_validate_path(char **filepath, int *hd_count_int);
int		ft_hd_init_file(char **filepath);
char	*ft_capture_in_interactive_mode(char *prompt);
void	heredoc_signal(void);

#endif