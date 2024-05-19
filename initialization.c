#include "pipex.h"
static void free_args(t_cmd *arr, int num);
static void	free_arr(char **arr);

t_cmd	*init_commands(int argc, char **argv, char **envp)
{
	char	**path;

	path = get_path(envp);
	if (!path)
		exit_failure();
	return (init_args(path, argc, argv));
}

t_cmd	*init_args(char **path, int argc, char **argv)
{
	int	i;
	t_cmd *arr;

	i = -1;
	arr = (t_cmd *)malloc(argc * sizeof(t_cmd));
	if (!arr)
		exit_failure();
	while (++i < argc)
	{
		arr[i].args = ft_split(argv[i], ' ');
		if (!arr[i].args)
		{
			free_args(arr, i);
			free_arr(path);
			exit_failure();
		}
		arr[i].path = parsing_path(path, arr[i].args[0]);
		if (!arr[i].path)
		{
			free_args(arr, i);
			free_arr(path);
			exit_failure();
		}
	}
	free_arr(path);
	return (arr);
}

static void free_args(t_cmd *arr, int num)
{
	int i;

	i = -1;
	while (++i < num)
	{
		free_arr(arr[i].args);
		free(arr[i].path);
	}
	free(arr);
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
			if (!tmp)
			{
				free_arr(path);
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
	char *cmd_path;

	i = 0;
	if (!path || !cmd)
		return (NULL);
	if (!access(cmd, F_OK))
	{
		cmd_path = (char *)malloc(sizeof(char) * ft_strlen(cmd) + 1);
		if (!cmd_path)
			return (NULL);
		ft_strlcpy(cmd_path, cmd, ft_strlen(cmd));
		return (cmd_path);
	}
	while (path[i])
	{
		cmd_path = ft_strjoin(path[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (!access(cmd_path, F_OK))
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

void free_pipex(t_pipex *pipex)
{
	int	i;

	i = -1;
	if (close(pipex->in_file))
		exit_failure();
	if (close(pipex->out_file))
		exit_failure();
	if (close(pipex->in_pipe[0]))
		exit_failure();
	if (close(pipex->in_pipe[1]))
		exit_failure();
	if (close(pipex->out_pipe[0]))
		exit_failure();
	if (close(pipex->out_pipe[1]))
		exit_failure();
	while(++i < pipex->cmds_num)
	{
		free(pipex->cmds[i].path);
		free_arr(pipex->cmds[i].args);
	}
	free(pipex);
}

t_pipex *init_pipex(t_cmd *arr, int argc, char **argv)
{
	t_pipex *pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
	{
		free_args(arr, argc - 3);
		exit_failure();
	}
	pipex->cmds = arr;
	pipex->cmds_num = argc - 3;
	pipex->in_file = open(argv[1], O_RDONLY);
	pipex->out_file = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (pipex->in_file == -1 || pipex->out_file == -1)
		exit_failure();
	if (pipe(pipex->in_pipe) == -1)
		exit_failure();
	if (pipe(pipex->out_pipe) == -1)
		exit_failure();
	return (pipex);
}