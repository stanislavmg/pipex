#include "pipex.h"
void	validation_args(int argc, char **argv);
int		exec_commands(t_pipex *pipex, char **envp);
t_pipex	*init_pipex(t_node *arr, int argc, char **argv);

int main(int argc, char **argv, char **envp)
{
	t_node *arr;

	validation_args(argc, argv);
	init_commands(argc - 1, argv + 1, envp, &arr);
	exec_commands(init_pipex(arr, argc, argv), envp);
	return (0);
}

int	exec_commands(t_pipex *pipex, char **envp)
{
	int		i;
 	int		ch;
	pid_t	pid;
	int		pdes1[2];
	int		pdes2[2];
 	char	buf[BUFFER_SIZE];

	i = 0;
	pipe(pdes1);
	pipe(pdes2);
	while (0 != (ch = read(pipex->input, buf, BUFFER_SIZE)))
		write(pdes1[1], buf, BUFFER_SIZE);
	while (i < pipex->count)
	{
		pid = fork();
		if (!pid)
		{
			printf("I'm child\n");
			dup2(pdes1[0], STDIN_FILENO);
			//dup2(pdes2[1], STDOUT_FILENO);
			execve(((pipex->cmds)[i]).name, ((pipex->cmds)[i]).args, envp);
		}
		waitpid(pid, NULL, __W_CONTINUED);
		printf("I'm here\n");
		while (0 > (ch = read(pdes2[0], buf, BUFFER_SIZE)))
		{
			printf("in while\n");
			write(pdes1[1], buf, BUFFER_SIZE);
		}
		i++;
	}
	return (0);
}

t_pipex	*init_pipex(t_node *arr, int argc, char **argv)
{
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		return (NULL);
	pipex->cmds = arr;
	pipex->count = argc - 3;
	pipex->input = open(argv[1], R_OK);
	pipex->output = open(argv[argc - 1], W_OK);
	if (pipex->input < 0)
		perror(strerror(pipex->input));
	else if (pipex->output < 0)
		perror(strerror(pipex->output));
	return (pipex);
}

// void	test_dup(int count, t_node *arr, char **envp)
// {
// 	int	pdes1[2];
// 	int	pdes2[2];
// 	pid_t	pid;
// 	char	buf[1024];
// 	int		ch;
// 	char	*cmd1 = "/usr/bin/ls";
// 	char	**arg1; char	**arg2;
// 	char	*cmd2 = "/usr/bin/wc";
	
// 	pipe(pdes1);
// 	pipe(pdes2);
// 	arg1 = malloc(sizeof(char *) * 3);
// 	arg1[0] = "ls";
// 	arg1[1] = "-la";
// 	arg1[2] = NULL;
// 	arg2 = malloc(sizeof(char *) * 3);
// 	arg2[0] = "wc";
// 	arg2[1] = "-l";
// 	arg2[2] = NULL;
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		close(pdes1[0]);
// 		close(pdes2[0]);
// 		close(pdes2[1]);
// 		dup2(pdes1[1], STDOUT_FILENO);
// 		execve(cmd1, arg1, envp);
// 	}
// 	wait(NULL);
// 	//write(1, buf, ch);
// 	//ch = read(pdes1[0], buf, 1024);
// 	//write(pdes2[1], buf, ch);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		// close(pdes2[1]);
// 		// close(pdes1[0]);
// 		// close(pdes1[1]);
// 		dup2(pdes1[0], STDIN_FILENO);
// 		execve(cmd2, arg2, envp);
// 	}
// 	//waitpid(pid, NULL, 0);
// }