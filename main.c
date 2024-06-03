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

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*arr;

	arr = NULL;
	if (argc != CMDS_NUM || !argv || !envp)
		exit(EXIT_FAILURE);
	arr = init_commands(argc - 3, argv + 2, envp);
	if (!arr)
		return (-1);
	exec_commands(arr, argv, envp);
	free_args(arr, 2);
	return (0);
}

int	first_ch(int *pdes, const char *fname, t_cmd *cmd, char **envp)
{
	int	fd;

	if (access(fname, F_OK) || access(fname, R_OK))
		exit_failure(fname, NULL);
	if (!cmd->path && cmd->args[0])
		exit_failure(cmd->args[0], CMD_ERR);
	if (!cmd->path[0])
		exit_failure(cmd->args[0], CMD_ERR);
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		exit(EXIT_FAILURE);
	ft_close(&pdes[0]);
	if (dup2(fd, STDIN_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	if (dup2(pdes[1], STDOUT_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	ft_close(&pdes[1]);
	if (cmd->path)
		execve(cmd->path, cmd->args, envp);
	exit_failure(cmd->args[0], NULL);
	return (1);
}

int	second_ch(int *pdes, const char *fname, t_cmd *cmd, char **envp)
{
	int	fd;

	if (!access(fname, F_OK) && access(fname, W_OK))
		exit_failure(fname, NULL);
	if (!cmd->path && cmd->args[0])
		exit_failure(cmd->args[0], CMD_ERR);
	if (!cmd->path[0])
		exit(EXIT_FAILURE);
	fd = open(fname, O_TRUNC | O_CREAT | O_RDWR, 0644);
	ft_close(&pdes[1]);
	if (dup2(pdes[0], STDIN_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	ft_close(&pdes[0]);
	if (cmd->path)
		execve(cmd->path, cmd->args, envp);
	exit_failure(cmd->args[0], NULL);
	return (1);
}

void	exec_commands(t_cmd *cmds, char **argv, char **envp)
{
	pid_t	ps1;
	pid_t	ps2;
	int		status;
	int		pdes[2];

	pipe(pdes);
	ps1 = fork();
	if (!ps1)
		first_ch(pdes, argv[1], cmds, envp);
	ps2 = fork();
	if (!ps2)
		second_ch(pdes, argv[4], ++cmds, envp);
	if (ps1 == -1 || ps2 == -1)
		return ;
	ft_close(&pdes[0]);
	ft_close(&pdes[1]);
	waitpid(ps1, &status, 0);
	waitpid(ps2, &status, 0);
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
	ft_strlcpy(cmd_path, cmd, ft_strlen(cmd) + 1);
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
