#include "pipex.h"

t_node	*init_commands(int argc, char **argv)
{
	t_node	*rval;
	int	i;

	i = -1;
	rval = (t_node *)malloc(argc * sizeof(t_node));
	if (!rval)
		return (NULL);
	while (++i < argc)
		get_args(rval + i, argv[i]);
	return (rval);
}

void	get_args(t_node *cmd, char *argv)
{
	char	**tmp;
	int		i;

	tmp = ft_split(argv, ' ');
	cmd->name = tmp[0];
	cmd->args = tmp + 1;
}

char **get_path(char **envp)
{
	char	**path;
	int		i;

	i = 0;
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	path = ft_split(*envp, ':');
	while (path[i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
			path[i] = ft_strjoin(path[i], "/");
		i++;
	}
	return (path);
}