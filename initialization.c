/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:47:59 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/05/19 11:48:02 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_cmd	*init_commands(int argc, char **argv, char **envp)
{
	char	**path;

	path = get_path(envp);
	if (!path)
		exit_failure(NULL, NULL);
	return (init_args(path, argc, argv));
}

t_cmd	*init_args(char **path, int argc, char **argv)
{
	int		i;
	t_cmd	*arr;

	i = -1;
	arr = (t_cmd *)malloc(argc * sizeof(t_cmd));
	if (!arr)
		exit_failure(NULL, NULL);
	while (++i <= argc)
	{
		if (!argv[i] || !argv[i][0])
		{
			arr[i].args = NULL;
			arr[i].path = NULL;
			continue ;
		}
		if (i == argc)
		{
			free_arr(path);
			return (arr);
		}
		arr[i].args = ft_split(argv[i], ' ');
		if (!arr[i].args || !arr[i].args[0])
			break ;
		arr[i].path = parsing_path(path, arr[i].args[0]);
	}
	free_args(arr, i);
	free_arr(path);
	exit_failure(NULL, NULL);
	return (NULL);
}

char	**get_path(char **envp)
{
	char	**path;
	char	*tmp;
	int		i;

	i = -1;
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	path = ft_split(*envp, ':');
	if (!path)
		return (NULL);
	while (path[++i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
		{
			tmp = ft_strjoin(path[i], "/");
			if (!tmp)
			{
				free_arr(path);
				return (NULL);
			}
			free(path[i]);
			path[i] = tmp;
		}
	}
	return (path);
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

t_pipex	*init_pipex(t_cmd *arr, int argc, char **argv)
{
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
	{
		free_args(arr, argc - 3);
		exit_failure(NULL, NULL);
	}
	pipex->cmds = arr;
	pipex->cmds_num = argc - 3;
	pipex->in_file = open(argv[1], O_RDONLY);
	pipex->out_file = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex->in_file == -1 || pipex->out_file == -1)
		exit_failure(NULL, NULL);
	if (pipe(pipex->in_pipe) == -1)
		exit_failure(NULL, NULL);
	if (pipe(pipex->out_pipe) == -1)
		exit_failure(NULL, NULL);
	return (pipex);
}
