#include "pipex.h"

void	validation_args(int argc, char **argv)
{
	if (argc != 5)
	{
		perror("Number of arguments not equal 4\n");
		exit(EXIT_FAILURE);
	}
	(void)argv;
	// else if (access(argv[1], F_OK | R_OK))
	// {
	// 	perror("Permission denied\n");
	// 	exit(EXIT_FAILURE);
	// }
	// else if (access(argv[argc - 1], F_OK | W_OK))
	// {
	// 	perror("Permission denied\n");
	// 	exit(EXIT_FAILURE);
	// }
}
