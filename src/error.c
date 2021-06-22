#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "libft/gc.h"

#include "pipex.h"

/*
** Returns the pointer passed as an argument if it is not
** NULL. Otherwise, the dynamically allocated memory is freed and the program
** is exited.
*/

void	*assert_ptr(void *p)
{
	if (p == NULL)
	{
		dprintf(STDERR_FILENO, "Dynamic memory allocation error\n");
		pipex_exit(1);
	}
	return (p);
}

/*
** Exit pipex, freeing all the dynamically allocated resources.
*/

void	pipex_exit(int exit_code)
{
	ft_gc_destroy(stat_get()->gc);
	exit(exit_code);
}
