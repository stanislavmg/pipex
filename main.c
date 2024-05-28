/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:48:07 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/05/19 11:48:09 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	tmain(int argc, char **argv, char **envp)
{
	t_cmd	*arr;
	t_pipex	*pipex;

	arr = NULL;
	pipex = NULL;
	if (argc < CMDS_NUM)
		exit(EXIT_FAILURE);
	if (!envp || !argv)
		return (-1);
	arr = init_commands(argc - 3, argv + 2, envp);
	if (!arr)
		return (-1);
	pipex = init_pipex(arr, argc, argv);
	if (!pipex)
	{
		free_args(arr, argc - 3);
		return (-1);
	}
	exec_commands(pipex, envp);
	return (0);
}

int	main(int ac, char **ar, char **en)
{
	tmain(ac, ar, en);
	system("leaks pipex");
	return (0);
}

int	create_child(int *in_pipe, int *out_pipe, t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		exit_failure(NULL, NULL);
	if (pid == 0)
	{
		if (ft_close(&in_pipe[1]) == -1 || ft_close(&out_pipe[0]) == -1)
			exit_failure(cmd->args[0], NULL);
		if (dup2(in_pipe[0], STDIN_FILENO) == -1)
			exit_failure(cmd->args[0], NULL);
		if (dup2(out_pipe[1], STDOUT_FILENO) == -1)
			exit_failure(cmd->args[0], NULL);
		if (cmd->path)
			execve(cmd->path, cmd->args, envp);
		else
			exit_failure(cmd->args[0], CMD_ERR);
		exit_failure(cmd->args[0], NULL);
	}
	ft_close(&in_pipe[0]);
	ft_close(&in_pipe[1]);
	ft_close(&out_pipe[1]);
	wait(&status);
	return (status);
}

void	exec_commands(t_pipex *pipex, char **envp)
{
	int		i;
	int		ch;
	char	buf[BUFFER_SIZE];

	i = -1;
	if (pipex->cmds[0].path && pipex->in_file)
		ch = read(pipex->in_file, buf, BUFFER_SIZE);
	else
		ch = 0;
	while (0 < ch)
	{
		write(pipex->in_pipe[1], buf, ch);
		ch = read(pipex->in_file, buf, BUFFER_SIZE);
	}
	while (++i < pipex->cmds_num)
	{
		create_child(pipex->in_pipe, pipex->out_pipe, pipex->cmds + i, envp);
		data_flow(pipex, buf, i);
	}
	free_pipex(pipex);
}

void	data_flow(t_pipex *pipex, char *buf, int count)
{
	int	ch;

	ch = read(pipex->out_pipe[0], buf, BUFFER_SIZE);
	pipe(pipex->in_pipe);
	while (ch > 0)
	{
		if (count + 1 == pipex->cmds_num)
			write(pipex->out_file, buf, ch);
		else
			write(pipex->in_pipe[1], buf, ch);
		ch = read(pipex->out_pipe[0], buf, BUFFER_SIZE);
	}
	ft_close(&pipex->out_pipe[0]);
	pipe(pipex->out_pipe);
}

char	*parsing_path(char **path, char *cmd)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (!path || !*path || !cmd)
		return (NULL);
	cmd_path = (char *)malloc(sizeof(char) * ft_strlen(cmd) + 1);
	if (!cmd_path)
		return (NULL);
	ft_strlcpy(cmd_path, cmd, ft_strlen(cmd));
	while (path[i])
	{
		if (!access(cmd_path, F_OK))
			return (cmd_path);
		free(cmd_path);
		cmd_path = ft_strjoin(path[i], cmd);
		if (!cmd_path)
			return (NULL);
		i++;
	}
	free(cmd_path);
	return (NULL);
}
