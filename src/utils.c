#include <unistd.h>

#include "pipex.h"

void	close_pipes(int *pipefd, int length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		close((pipefd + (i * 2))[0]);
		close((pipefd + (i * 2))[1]);
		++i;
	}
}
