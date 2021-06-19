#include "lib.h"
#include "pipex.h"

t_argv	*command_parse(char *arg)
{
	t_argv	*cmd_data;
	char	*token;

	cmd_data = argv_new(10);
	token = ft_strtok(arg, " \t");
	while (token != NULL)
	{
		argv_append(cmd_data, token);
		token = ft_strtok(NULL, " \t");
	}
	return (cmd_data);
}
