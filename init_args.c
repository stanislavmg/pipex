#include "pipex.h"

void	init_commands(int argc, char **argv, char **envp, t_node **arr)
{
	int		i;
	char	**path;

	i = -1;
	*arr = (t_node *)malloc(argc * sizeof(t_node));
	if (!*arr)
		exit(EXIT_FAILURE);
	path = get_path(envp);
	if (!path)
	{
		free(*arr);
		exit(EXIT_FAILURE);
	}
	while (++i < argc)
	{
		get_args(*arr + i, argv[i]);
		(*arr)[i].name = parsing_path(path, (*arr)[i].name);
	}
	free(path);
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
	char	*tmp;
	int		i;

	i = 0;
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	path = ft_split(*envp, ':');
	while (path[i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
		{
			tmp = path[i];
			path[i] = ft_strjoin(path[i], "/");
			free(tmp);
		}
		i++;
	}
	return (path);
}