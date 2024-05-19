#include "pipex.h"

void	exit_failure(void)
{
	perror(strerror(errno));
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
	t_cmd *arr;

	if (!envp || !argv)
		return (-1);
	validation_args(argc, argv);
	arr = init_commands(argc - 3, argv + 2, envp);
	exec_commands(init_pipex(arr, argc, argv), envp);
	return (0);
}

int create_child(int *in_pipe, int *out_pipe, t_cmd *cmd, char **envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
		exit_failure();
	if (pid == 0)
	{
		if (close(in_pipe[1]) == -1 || close(out_pipe[0]) == -1)
			exit_failure();
		if (dup2(in_pipe[0], STDIN_FILENO) == -1)
			exit_failure();
		if (dup2(out_pipe[1], STDOUT_FILENO) == -1)
			exit_failure();
		execve(cmd->path, cmd->args, envp);
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(in_pipe[0]);
	close(in_pipe[1]);
	close(out_pipe[1]);
	wait(&status);
	return (status);
}

void exec_commands(t_pipex *pipex, char **envp)
{
	int i;
	int ch;
	int status;
	char buf[BUFFER_SIZE];

	i = -1;
	while (0 < (ch = read(pipex->in_file, buf, BUFFER_SIZE)))
		write(pipex->in_pipe[1], buf, ch);
	while (++i < pipex->cmds_num)
	{
		status = create_child(pipex->in_pipe, pipex->out_pipe, pipex->cmds + i, envp);
		if (status)
		{
			printf("hello\n");
			close(pipex->out_pipe[0]);
			free_pipex(pipex);
			exit_failure();
		}
		pipe(pipex->in_pipe);
		while ((ch = read(pipex->out_pipe[0], buf, BUFFER_SIZE)) > 0)
		{
			if (i + 1 == pipex->cmds_num)
				write(pipex->out_file, buf, ch);
			else
				write(pipex->in_pipe[1], buf, ch);
		}
		close(pipex->out_pipe[0]);
		pipe(pipex->out_pipe);
	}
	free_pipex(pipex);
}