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
