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

typedef struct s_node
{
	char	*name;
	char	**args;
} t_node;

typedef struct s_pipex
{
	int 	input;
	int 	output;
	int		count;
	t_node	*cmds;
} t_pipex;

t_node	*init_commands(int argc, char **argv, char **envp);
t_pipex *init_pipex(t_node *arr, int argc, char **argv);
t_node	*init_args(char **path, int argc, char **argv);
char 	**get_path(char **envp);
char	*parsing_path(char **path, char *cmd);
void	validation_args(int argc, char **argv);
void	exec_commands(t_pipex *pipex, char **envp);

#endif