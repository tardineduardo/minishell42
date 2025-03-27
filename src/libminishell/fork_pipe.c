#include "./include/prototype.h"

int	*ft_pipe_control(void)
{	
	int	*pipefd;

	pipefd = (int *)malloc(sizeof(int *) * 2);
    if (!pipefd || pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    return (pipefd);
}

pid_t	ft_fork_control(void)
{
	pid_t	cpid;

	cpid = fork();
	if (cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (cpid);
}
