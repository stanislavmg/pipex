#include "pipex.h"
void 	validation_args(int argc, char **argv);
void 	free_pipex(t_pipex *pipex);
void	 exec_commands(t_pipex *pipex, char **envp);
t_pipex *init_pipex(t_node *arr, int argc, char **argv);
void 	create_child(int *pdes1, int *pdes2, t_node *cmd, char **envp);

int main(int argc, char **argv, char **envp)
{
	t_node *arr;

	validation_args(argc, argv);
	arr = init_commands(argc - 3, argv + 2, envp);
	exec_commands(init_pipex(arr, argc, argv), envp);
	return (0);
}

void create_child(int *pdes1, int *pdes2, t_node *cmd, char **envp)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		close(pdes1[1]);
		close(pdes2[0]);
		dup2(pdes1[0], STDIN_FILENO);
		dup2(pdes2[1], STDOUT_FILENO);
		execve(cmd->name, cmd->args, envp);
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void exec_commands(t_pipex *pipex, char **envp)
{
	int i;
	int ch;
	int	status;
	int pdes1[2];
	int pdes2[2];
	char buf[BUFFER_SIZE];

	i = -1;
	pipe(pdes1);
	pipe(pdes2);
	while (0 != (ch = read(pipex->input, buf, BUFFER_SIZE)))
		write(pdes1[1], buf, ch);
	while (++i < pipex->count)
	{
		create_child(pdes1, pdes2, pipex->cmds + i, envp);
		close(pdes1[0]);
		close(pdes1[1]);
		close(pdes2[1]);
		wait(&status);
		if (status)
		{
			close(pdes2[0]);
			free_pipex(pipex);
		}
		pipe(pdes1);
		while ((ch = read(pdes2[0], buf, BUFFER_SIZE)) > 0)
		{
			if (i + 1 == pipex->count)
			{
				write(pipex->output, buf, ch);
				close(pdes2[0]);
				close(pdes1[0]);
				close(pdes1[1]);
				break ;
			}
			else
				write(pdes1[1], buf, ch);
		}
		close(pdes2[0]);
		pipe(pdes2);
	}
}

t_pipex *init_pipex(t_node *arr, int argc, char **argv)
{
	t_pipex *pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		return (NULL);
	pipex->cmds = arr;
	pipex->count = argc - 3;
	pipex->input = open(argv[1], O_RDONLY);
	pipex->output = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex->input < 0)
		perror(strerror(pipex->input));
	else if (pipex->output < 0)
		perror(strerror(pipex->output));
	return (pipex);
}