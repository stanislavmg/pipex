#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_node	*arr;
	char	**path;
	int		pid;
	int pipedes[2];
	int	i, j;

	arr = init_commands(argc - 1, argv + 1);
	path = get_path(envp);
	if (!arr)
		return (1);
	i = 0;
	// while (i < argc - 1)
	// {
	// 	printf("name = %s\n", arr[i].name);
	// 	j = 0;
	// 	while ((arr[i]).args[j])
	// 	{
	// 		printf("%s\n", arr[i].args[j]);
	// 		j++;
	// 	}
	// 	i++;
	// }
	while (*path)
	{
		printf("%s\n", *path);
		path++;
	}
	return (0);
}

void	rec_fork(int count, char **cmd, char **arg)
{
	int pipedes[2];
	pid_t	pid;

	if (count == -1)
		return ;
	pipe(pipedes);
	pid = fork();
	if ( pid == 0 ) 
	{
		rec_fork(count - 1, cmd + 1, arg + 1);
		close(pipedes[0]);
		dup2(pipedes[1], 1);
		execve(cmd, arg, NULL);
		close(pipedes[1]);
	}
	else
	{
		wait();
		char buf[1024];
		int len;
		close(pipedes[1]);
		while ((len = read(pipedes[0], buf, 1024)) != 0)
			write(2, buf, len);
		close(pipedes[0]);
	}
	return ;
}