#include <unistd.h>

#include "libft/cstring.h"

#include "pipex.h"

void	close_two(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void	close_pipes(int *pipefd, int length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		close_two(pipefd + (i * 2));
		++i;
	}
}

void	pipeline_destroy(t_vector pipeline)
{
	ft_vector_destroy(pipeline, (void *)(void *)&command_destroy);
}

void	command_destroy(t_vector cmd)
{
	ft_vector_destroy(cmd, NULL);
}

t_vector	command_parse(char *arg)
{
	t_vector	cmd;
	char		*token;

	cmd = ft_vector_new(10);
	if (cmd == NULL)
		return (NULL);
	token = ft_strtok(arg, " \t");
	while (token != NULL)
	{
		ft_vector_append(cmd, token);
		token = ft_strtok(NULL, " \t");
	}
	return (cmd);
}
