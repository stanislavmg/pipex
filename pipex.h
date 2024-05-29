/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:48:18 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/05/19 11:48:19 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# define BUFFER_SIZE 1024
# define CMD_ERR "command not found\n"
# define CMDS_NUM 5

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_cmd
{
	char	*path;
	char	**args;
}	t_cmd;

/* initialization structures */
t_cmd	*init_commands(int argc, char **argv, char **envp);
t_cmd	*init_args(char **path, int argc, char **argv);
char	**get_path(char **envp);
char	*parsing_path(char **path, char *cmd);
/* execution commands */
int	first_ch(int *pdes, const char *fname, t_cmd *cmd, char **envp);
int	second_ch(int *pdes, const char *fname, t_cmd *cmd, char **envp);
void	exec_commands(t_cmd *cmds, char **argv, char **envp);
/* errors handle and free structures */
void	exit_failure(const char *cmd, const char *message);
void	free_args(t_cmd *arr, int num);
void	free_arr(char **arr);
int		ft_close(int *fd);

#endif
