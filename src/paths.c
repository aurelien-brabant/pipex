#include <stdlib.h>
#include <stdio.h>

#include "pipex.h"

static void		populate_paths(t_argv *paths, char *path_value)
{
	char	*token;

	token = ft_strtok(path_value, ":");
	while (token != NULL)
	{
		argv_append(paths, token);
		token = ft_strtok(NULL, ":");
	}
}

char	*get_cmd_path(const char *cmd, const char *path)
{
	char	*cmd_path;
	size_t	i;

	cmd_path = malloc(sizeof (*cmd_path) + ft_strlen(cmd)
			+ ft_strlen(path) + 2);
	if (cmd_path == NULL)
		return (NULL);
	i = 0;
	while (*path != '\0')
		cmd_path[i++] = *path++;
	cmd_path[i++] = '/';
	while (*cmd != '\0')
		cmd_path[i++] = *cmd++;
	cmd_path[i] = '\0';
	return (cmd_path);
}

t_argv	*get_paths(char **env)
{
	t_argv	*paths;
	char	*path_value;
	char	*token;

	paths = argv_new(10);
	path_value = NULL;
	while (*env)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
			path_value = ft_strdup(*env + 5);
		++env;
	}
	if (path_value == NULL)
		printf("PATH is not set!\n");
	populate_paths(paths, path_value);
	return (paths);
}
