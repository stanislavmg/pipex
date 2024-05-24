/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:48:23 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/05/19 11:48:24 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validation_args(int argc, char **argv)
{
	// int	i;
	
	// i = 0;
	if (argc != CMDS_NUM)
	{
		perror("Number of arguments not equal 4\n");
		exit(EXIT_FAILURE);
	}
	// while (++i < argc)
	// {
	// 	if (!argv[i] || !argv[i][0])
	// 		exit(EXIT_FAILURE);
	// }
	if (access(argv[1], F_OK) || access(argv[1], R_OK))
	{
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	if (!access(argv[argc - 1], F_OK) && access(argv[argc - 1], W_OK))
	{
		perror(argv[argc - 1]);
		exit(EXIT_FAILURE);
	}
}
