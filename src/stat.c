#include "pipex.h"

t_stat	*stat_get(void)
{
	static t_stat	stat = {
		.envp = NULL,
		.paths = NULL,
		.mode = MODE_NORMAL,
		.filein = NULL,
		.fileout = NULL,
	};

	return (&stat);
}
