#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "libft/gc.h"
#include "libft/vector.h"
#include "libft/io.h"
#include "libft/cstring.h"

#include "pipex.h"

static void	pipeline_destroy(t_vector pipeline)
{
	ft_vector_destroy(pipeline, (void *)(void *)&argv_destroy);
}

t_vector	parsecl(int argc, char **argv)
{
	t_vector	pipeline;
	size_t		i;

	i = 0;
	if (argc < 5)
	{
		ft_dprintf(STDERR_FILENO, "Usage: pipex [here_doc] <in_arg> "
				"<cmd1> <cmd2> [...] <cmdn> <out_arg>\n"); 
		pipex_exit(1);
	}
	pipeline = ft_gc_add(stat_get()->gc, assert_ptr(ft_vector_new(10)),
			&pipeline_destroy);
	if (ft_strcmp(argv[i], HERE_DOC_MODE_WORD) == 0)
	{
		if (argc < 6)
		{
			ft_dprintf(STDERR_FILENO, "here_doc mode requires at least"
					"5 arguments, got %d\n", argc);
			pipex_exit(1);
		}
		++i;
	}
	stat_get()->in_arg = argv[i++];
	while (i < argc - 1)
		ft_vector_append(pipeline, assert_ptr(command_parse(argv[i++])));
	stat_get()->out_arg = argv[i];
	return (pipeline);
}