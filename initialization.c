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
		return (NULL);
	while (++i < argc)
	{
		arr[i].args = ft_split(argv[i], ' ');
		arr[i].path = parsing_path(path, arr[i].args[0]);
	}
	free_arr(path);
	return (arr);
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
	open_files(pipex, argc, argv);
	if (pipe(pipex->in_pipe) == -1)
	{
		free_pipex(pipex);
		exit_failure(NULL, NULL);
	}
	if (pipe(pipex->out_pipe) == -1)
	{
		free_pipex(pipex);
		exit_failure(NULL, NULL);
	}
	return (pipex);
}

void	open_files(t_pipex *pipex, int argc, char **argv)
{
	if (access(argv[1], F_OK) || access(argv[1], R_OK))
	{
		perror(argv[1]);
		pipex->in_file = 0;
	}
	else
		pipex->in_file = open(argv[1], O_RDONLY);
	if (!access(argv[argc - 1], F_OK) && access(argv[argc - 1], W_OK))
	{
		perror(argv[argc - 1]);
		pipex->out_file = 1;
	}
	else
		pipex->out_file = open(argv[argc - 1],
				O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex->in_file == -1)
	{
		perror("pipex");
		pipex->in_file = 0;
	}
	if (pipex->out_file == -1)
	{
		perror("pipex");
		pipex->out_file = 1;
	}
}
