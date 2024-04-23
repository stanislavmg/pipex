#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>

typedef struct s_node
{
	char			*name;
	char			**args;
} t_node;

t_node	*init_commands(int argc, char **argv);
void	get_args(t_node *cmd, char *argv);
char 	**get_path(char **envp);
void	rec_fork(int count, char **cmd, char **arg);

#endif