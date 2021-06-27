#include <unistd.h>

#include "libft/gc.h"
#include "libft/vector.h"
#include "libft/io.h"

#include "pipex.h"

int	main(int argc, char **argv, char *envp[])
{
	t_gc		gc;
	t_vector	pipeline;

	gc = ft_gc_new();
	if (gc == NULL)
	{
		ft_dprintf(STDERR_FILENO, "Error allocating garbage collector\n");
		return (1);
	}
	stat_get()->gc = gc;
	stat_get()->envp = envp;
	pipeline = parsecl(argc, argv);
	stat_get()->paths = get_paths(envp);
	execute_pipeline(pipeline);
	pipex_exit(0);
}
