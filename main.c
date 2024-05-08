#include "pipex.h"
void	validation_args(int argc, char *argv[]);

int main(int argc, char **argv, char **envp)
{
	t_node *arr;

	validation_args(argc, argv);
	init_commands(argc - 1, argv + 1, envp, &arr);
	return (0);
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