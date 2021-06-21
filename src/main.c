#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>

#include "libft/gc.h"
#include "libft/vector.h"
#include "libft/io.h"

#include "pipex.h"

static void	execute(t_argv *cmd, int pipefd[2], int index, int length)
{
	if (stat_get()->mode == MODE_NORMAL)
		execute_normal(cmd, pipefd, index, length);
	else
		execute_here_doc(cmd, pipefd, index, length);
}

static void	execute_from_path(t_argv *cmd, int *pipefd, int index, int length)
{
	t_argv	*paths;
	char	*cmd_path;
	int		pid;
	size_t	i;

	i = 0;
	paths = stat_get()->paths;
	while (i < paths->length)
	{
		cmd_path = get_cmd_path(cmd->args[0], paths->args[i]);
		if (access(cmd_path, X_OK) == 0)
		{
			cmd->args[0] = cmd_path;
			execute(cmd, pipefd, index, length);
			free(cmd_path);
			return ;
		}
		free(cmd_path);
		++i;
	}
	dprintf(STDERR_FILENO, "pipex: %s: command not found\n", cmd->args[0]);
}

static void	execute_pipeline(t_vector pipeline, int *pipefd)
{
	size_t	i;
	t_argv	*cmd;
	int		*cur_pipefd;

	i = 0;
	while (i < ft_vector_length(pipeline))
	{
		cur_pipefd = pipefd + (i * 2);
		pipe(cur_pipefd);
		cmd = ft_vector_get(pipeline, i);
		if (access(cmd->args[0], X_OK) == 0)
			execute(cmd, pipefd + (i * 2), i, ft_vector_length(pipeline));
		else
			execute_from_path(cmd, pipefd + (i * 2), i,
				ft_vector_length(pipeline));
		close_pipe(cur_pipefd, i, ft_vector_length(pipeline));
		++i;
	}
}

void	close_pipe(int pipefd[2], int index, int length)
{
	if (index != 0 && pipefd[-2] != -1)
	{
		close(pipefd[-2]);
		pipefd[-2] = -1;
	}
	close(pipefd[1]);
}

int	main(int argc, char **argv, char *envp[])
{
	t_gc		gc;
	t_vector	pipeline;
	int			*pipefd;
	size_t		i;

	gc = ft_gc_new();
	if (gc == NULL)
		return (1);
	stat_get()->gc = gc;
	stat_get()->envp = envp;
	pipeline = parsecl(argc, argv);
	stat_get()->paths = get_paths(envp);
	pipefd = ft_gc_add(stat_get()->gc, assert_ptr(
				malloc(sizeof (int) * ft_vector_length(pipeline) * 2)), &free);
	i = 0;
	execute_pipeline(pipeline, pipefd);
	pipex_exit(0);
	return (0);
}
