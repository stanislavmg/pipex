#ifndef PIPEX_H
# define PIPEX_H
# define BUFFER_SIZE 1024

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
} t_cmd;

typedef struct s_pipex
{
	int		in_pipe[2];
	int		out_pipe[2];
	int 	in_file;
	int 	out_file;
	int		cmds_num;
	t_cmd	*cmds;
} t_pipex;

t_cmd	*init_commands(int argc, char **argv, char **envp);
t_pipex *init_pipex(t_cmd *arr, int argc, char **argv);
t_cmd	*init_args(char **path, int argc, char **argv);
char 	**get_path(char **envp);
char	*parsing_path(char **path, char *cmd);
void	validation_args(int argc, char **argv);
void	exec_commands(t_pipex *pipex, char **envp);
void 	free_pipex(t_pipex *pipex);
void	exec_commands(t_pipex *pipex, char **envp);
int 	create_child(int *pdes1, int *pdes2, t_cmd *cmd, char **envp);
void	exit_failure(void);

#endif