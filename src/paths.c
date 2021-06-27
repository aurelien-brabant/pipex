#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "libft/cstring.h"
#include "libft/io.h"

#include "pipex.h"

static void	populate_paths(t_vector paths, char *path_value)
{
	char	*token;

	token = ft_strtok(path_value, ":");
	while (token != NULL)
	{
		ft_vector_append(paths, token);
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

t_vector	get_paths(char **env)
{
	t_vector	paths;
	char		*path_value;
	char		*token;

	paths = ft_gc_add(stat_get()->gc, assert_ptr(ft_vector_new(10)),
			(void *)(void *)&ft_vector_destroy);
	path_value = NULL;
	while (*env)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
			path_value = *env + 5;
		++env;
	}
	if (path_value == NULL)
		ft_dprintf(STDERR_FILENO, "PATH is not set!\n");
	else
		populate_paths(paths, path_value);
	return (paths);
}
