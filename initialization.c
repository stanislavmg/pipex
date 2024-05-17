#include "pipex.h"
static void free_args(t_node *arr, int num);
static void	free_arr(char **arr);

t_node	*init_commands(int argc, char **argv, char **envp)
{
	char	**path;

	path = get_path(envp);
	if (!path)
		exit(EXIT_FAILURE);
	return (init_args(path, argc, argv));
}

t_node	*init_args(char **path, int argc, char **argv)
{
	int	i;
	t_node *arr;

	i = -1;
	arr = (t_node *)malloc(argc * sizeof(t_node));
	if (!arr)
	{
		perror("No memory!");
		exit(EXIT_FAILURE);
	}
	while (++i < argc)
	{
		arr[i].args = ft_split(argv[i], ' ');
		if (!arr[i].args)
			free_args(arr, i);
		arr[i].name = arr[i].args[0];
		arr[i].name = parsing_path(path, arr[i].name);
		if (!arr[i].name)
		{
			free_args(arr, i);
			exit(EXIT_FAILURE);
		}
	}
	free(path);
	return (arr);
}

static void free_args(t_node *arr, int num)
{
	int i;

	i = -1;
	while (++i < num - 1)
	{
		free_arr(arr[i].args);
		free(arr[i].name);
	}
	free(arr);
	exit (EXIT_FAILURE);
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
	if (!path)
		return (NULL);
	while (path[i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
		{
			tmp = ft_strjoin(path[i], "/");
			if (!path[i])
			{
				free_arr(path);
				free(tmp);
				return (NULL);
			}
			free(path[i]);
			path[i] = tmp;
		}
		i++;
	}
	return (path);
}

static void	free_arr(char **arr)
{
	int	i;

	i = -1;
	if (!arr || !*arr)
		return ;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

char	*parsing_path(char **path, char *cmd)
{
	int	i;
	int	errno;
	char *cmd_path;

	i = 0;
	while (path[i])
	{
		cmd_path = ft_strjoin(path[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (!(errno = access(cmd_path, F_OK)))
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	perror(strerror(errno));
	return (NULL);
}

void free_pipex(t_pipex *pipex)
{
	int	i;

	i = -1;
	close(pipex->input);
	close(pipex->output);
	while(++i < pipex->count)
	{
		free(pipex->cmds[i].name);
		free(pipex->cmds[i].args);
	}
	free(pipex);
}