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

void	exit_failure(char *cmd, char *message)
{
	if (message)
	{
		write(2, "pipex: ", 7);
		ft_putstr_fd(cmd, 2);
		write(2, ": ", 2);
		ft_putchar_fd(message, 2);
	}
	else
		perror(cmd);
	exit(EXIT_FAILURE);
}