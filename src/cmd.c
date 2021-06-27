#include "libft/cstring.h"

#include "pipex.h"

t_vector	command_parse(char *arg)
{
	t_vector	cmd;
	char	*token;

	cmd = ft_vector_new(10);
	token = ft_strtok(arg, " \t");
	while (token != NULL)
	{
		ft_vector_append(cmd, token);
		token = ft_strtok(NULL, " \t");
	}
	return (cmd);
}
