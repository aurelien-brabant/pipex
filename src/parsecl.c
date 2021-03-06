#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "libft/gc.h"
#include "libft/vector.h"
#include "libft/io.h"
#include "libft/cstring.h"

#include "pipex.h"

static void	parse_here_doc_mode(int argc, char *arg, size_t *i)
{
	if (ft_strcmp(arg, HERE_DOC_MODE_WORD) == 0)
	{
		if (argc < 6)
		{
			ft_dprintf(STDERR_FILENO, "here_doc mode requires at least"
				" 4 arguments, got %d\n", argc - 2);
			pipex_exit(1);
		}
		stat_get()->mode = MODE_HERE_DOC;
		(*i)++;
	}
}

/*
** Parse the command line
** Generate a vector of commands that represents an actual pipeline.
*/

t_vector	parsecl(int argc, char **argv)
{
	t_vector	pipeline;
	size_t		i;

	i = 1;
	if (argc < 5)
	{
		ft_dprintf(STDERR_FILENO, PIPEX_USAGE);
		pipex_exit(1);
	}
	pipeline = ft_gc_add(stat_get()->gc, assert_ptr(ft_vector_new(10)),
			&pipeline_destroy);
	parse_here_doc_mode(argc, argv[i], &i);
	stat_get()->in_arg = argv[i++];
	while (i < argc - 1)
		ft_vector_append(pipeline, assert_ptr(command_parse(argv[i++])));
	stat_get()->out_arg = argv[i];
	return (pipeline);
}
