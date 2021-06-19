#include "pipex.h"

t_stat	*stat_get(void)
{
	static t_stat	stat = {
		.envp = NULL,
		.paths = NULL,
	};
	return (&stat);
}
