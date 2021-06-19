#include <stdlib.h>
#include <stdio.h>

#include "pipex.h"

char	**get_paths(char **env)
{
	char	*path_value;

	path_value = NULL;
	while (*env)
	{
		if (ft_strncmp(*env, "PATH", 4) == 0 && *(*env + 4) == '=')
			path_value = ft_strdup(*env + 5);
		++env;
	}
	if (path_value == NULL)
		printf("PATH is not set!");
	free(path_value);
	return (NULL);
}
