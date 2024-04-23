#include "pipex.h"

int main(int argc, char **argv, char **envp)
{
	t_node *arr;
	int pid;
	int i, j;

	init_commands(argc - 1, argv + 1, envp, &arr);
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
	// while (*path)
	// {
	// 	printf("%s\n", *path);
	// 	path++;
	// }
	// printf("%s\n", parsing_path(path, arr[0].name));
	rec_fork(argc - 1, arr);
	return (0);
}

void rec_fork(int count, t_node *arr)
{
	int pipedes[2];
	pid_t pid;
	int	status;

	printf("execute %s", arr->name);
	if (count == -1)
		return;
	pipe(pipedes);
	pid = fork();
	if (pid == 0)
	{
		rec_fork(count - 1, arr + 1);
		// close(pipedes[0]);
		dup2(pipedes[1], 1);
		printf("execute %s", arr->name);
		execve(arr->name, arr->args, NULL);
		// close(pipedes[1]);
	}
	else if (count)
	{
		waitpid(pid, &status, 0);
		// wait(NULL);
		char buf[1024];
		int len;
		// close(pipedes[1]);
		while ((len = read(pipedes[0], buf, 1024)) != 0)
			write(1, buf, len);
		// close(pipedes[0]);
	}
	return;
}