#include "pipex.h"

int	ft_close(int *fd)
{
	if (*fd == -1)
		return (0);
	if (close(*fd) == -1)
		return (-1);
	*fd = -1;
	return (0);
}

void	exit_failure(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}