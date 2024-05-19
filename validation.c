#include "pipex.h"

void	validation_args(int argc, char **argv)
{
	if (argc != 5)
	{
		perror("Number of arguments not equal 4\n");
		exit(EXIT_FAILURE);
	}
	if (access(argv[1], F_OK) || access(argv[1], R_OK))
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (!access(argv[argc - 1], F_OK) && access(argv[argc - 1], W_OK))
	{
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
}
