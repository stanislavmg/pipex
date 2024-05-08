#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_node
{
	char			*name;
	char			**args;
} t_node;

void	init_commands(int argc, char **argv, char **envp, t_node **arr);
void	get_args(t_node *cmd, char *argv);
char 	**get_path(char **envp);
char	*parsing_path(char **path, char *cmd);
void	test_dup(int count, t_node *arr, char **envp);
void	validation_args(int argc, char *argv[]);
/* unit test functions */

#endif