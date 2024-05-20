/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:59:19 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/05/19 11:59:22 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void free_pipex(t_pipex *pipex)
{
	int i;

	i = -1;
	if (ft_close(&pipex->in_file))
		exit_failure(NULL);
	if (ft_close(&pipex->out_file))
		exit_failure(NULL);
	if (ft_close(&pipex->in_pipe[0]))
		exit_failure(NULL);
	if (ft_close(&pipex->in_pipe[1]))
		exit_failure(NULL);
	if (ft_close(&pipex->out_pipe[0]))
		exit_failure(NULL);
	if (ft_close(&pipex->out_pipe[1]))
		exit_failure(NULL);
	while (++i < pipex->cmds_num)
	{
		free(pipex->cmds[i].path);
		free_arr(pipex->cmds[i].args);
	}
	free(pipex->cmds);
	free(pipex);
}

void free_arr(char **arr)
{
	int i;

	i = -1;
	if (!arr || !*arr)
		return;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

void free_args(t_cmd *arr, int num)
{
	int i;

	i = -1;
	if (!arr)
		return;
	while (++i < num)
	{
		free_arr(arr[i].args);
		free(arr[i].path);
	}
	free_arr(arr[i].args);
	free(arr);
}
