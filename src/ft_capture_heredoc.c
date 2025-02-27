#include "../include/minishell.h"

typedef struct s_heredoc
{
	char	*heredoc_file;
}			t_heredoc;

char *ft_create_filepath(int *heredoc_count_int)
{
	char		*heredoc_count_str;
	char		*heredoc_filepath;
	char		*heredoc_temp;	

	heredoc_count_str = ft_itoa(*heredoc_count_int);
	if (!heredoc_count_str)
		return (NULL);
	heredoc_filepath = ft_strjoin("/tmp/minishell_temp_heredoc_", heredoc_count_str);
	//ft_validate_heredoc_filepath(heredoc_filepath, &heredoc_count_int);
	
	free(heredoc_count_str);
	return(heredoc_filepath);
}

//t_list heredocs must be initialized!!!!!!!!!!!!!!!!!!!!!!!!!
void	ft_capture_heredoc(char *delimiter, t_list **heredocs)
{
	static int	heredoc_count_int;
	char		*heredoc_filepath;

	assert(delimiter);		// REMOVE LATER

	heredoc_filepath = ft_create_filepath(&heredoc_count_int);

	// TODO:
	// init node
	// add to end of t_list

	free(heredoc_filepath);	
	heredoc_count_int++;

}


// void	ft_validate_heredoc_filepath(char *heredoc_filepath, int *heredoc_count_int)
// {
// 	char		*heredoc_count_str;

// 	while (1)
// 	{
// 		if (access(heredoc_filepath, W_OK))
// 			break ;
// 		else
// 		{
// 			*heredoc_count_int++;
// 			heredoc_count_str = ft_itoa(*heredoc_count_int);
// 			if (!heredoc_count_str)
// 				return (NULL);			
// 			heredoc_filepath = ft_strjoin("/tmp/minishell_temp_heredoc_", heredoc_count_str);
// 		}
// 	}
// 	return ;
// }