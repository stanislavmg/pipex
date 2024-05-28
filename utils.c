/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:05:03 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/05/28 13:05:06 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_pipex(t_pipex *pipex)
{
	int	i;

	i = -1;
	if (ft_close(&pipex->in_file))
		perror("pipex");
	if (ft_close(&pipex->out_file))
		perror("pipex");
	if (ft_close(&pipex->in_pipe[0]))
		perror("pipex");
	if (ft_close(&pipex->in_pipe[1]))
		perror("pipex");
	if (ft_close(&pipex->out_pipe[0]))
		perror("pipex");
	if (ft_close(&pipex->out_pipe[1]))
		perror("pipex");
	while (++i < pipex->cmds_num)
	{
		if (pipex->cmds[i].path)
			free(pipex->cmds[i].path);
		free_arr(pipex->cmds[i].args);
	}
	free(pipex->cmds);
	free(pipex);
}

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	if (!arr || !*arr)
		return ;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

void	free_args(t_cmd *arr, int num)
{
	int	i;

	i = -1;
	if (!arr)
		return ;
	while (++i < num)
	{
		if (arr[i].args)
			free_arr(arr[i].args);
		if (arr[i].path)
			free(arr[i].path);
	}
	free(arr);
}

int	ft_close(int *fd)
{
	if (*fd == -1)
		return (0);
	if (close(*fd) == -1)
		return (-1);
	*fd = -1;
	return (0);
}

void	exit_failure(char *cmd, char *message)
{
	if (message)
	{
		write(2, "pipex: ", 7);
		ft_putstr_fd(cmd, 2);
		write(2, ": ", 2);
		ft_putstr_fd(message, 2);
	}
	else
		perror(cmd);
	exit(EXIT_FAILURE);
}
