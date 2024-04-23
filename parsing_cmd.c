#include "pipex.h"

char	*parsing_path(char **path, char *cmd)
{
	int	i;
	char *cmd_path;

	i = 0;
	while (path[i])
	{
		cmd_path = ft_strjoin(path[i], cmd);
		if (!access(cmd_path, F_OK))
		{
			free(cmd);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	return (NULL);
}