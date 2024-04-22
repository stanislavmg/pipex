#include "libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct s_cmd{
	char			*name;
	char			**args;
} t_list;

t_list	*init_commands(int argc, char **argv);
void	get_args(t_list *cmd, char *argv);

int	main(int argc, char **argv, char **env)
{
	t_list	*arr;
	int		pid;
	int pipedes[2];

	arr = init_commands(argc - 1, argv + 1);
	if (!arr)
		return (1);
	
	return (0);
}

t_list	*init_commands(int argc, char **argv)
{
	t_list	*rval;
	int	i;

	i = -1;
	rval = (t_list *)malloc(argc * sizeof(t_list));
	if (!rval)
		return (NULL);
	while (++i < argc)
		get_args(rval + i, argv[i]);
	return (rval);
}

void	get_args(t_list *cmd, char *argv)
{
	char	**tmp;
	int		i;

	tmp = ft_split(argv, ' ');
	cmd->name = tmp[0];
	cmd->args = tmp + 1;
}

void	rec_fork(int count, char **cmd, char **arg)
{
	int pipedes[2];
	pid_t	pid;

	if (count == -1)
		return ;
	pipe(pipedes);
	pid = fork();
	if ( pid == 0 ) 
	{
		rec_fork(count - 1, cmd + 1, arg + 1);
		close(pipedes[0]);
		dup2(pipedes[1], 1);
		execve(cmd, arg, NULL);
		close(pipedes[1]);
	}
	else
	{
		wait();
		char buf[1024];
		int len;
		close(pipedes[1]);
		while ((len = read(pipedes[0], buf, 1024)) != 0)
			write(2, buf, len);
		close(pipedes[0]);
	}
	return ;
}