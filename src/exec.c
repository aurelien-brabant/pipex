#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "libft/io.h"

#include "pipex.h"

static void	execute(t_vector cmd)
{
	if (execve(ft_vector_get(cmd, 0), (char **)ft_vector_get_array(cmd),
			stat_get()->envp) == -1)
	{
		ft_dprintf(STDERR_FILENO, "pipex: %s: %s\n", ft_vector_get(cmd, 0),
			strerror(errno));
		pipex_exit(126);
	}
}

static void	execute_from_path(t_vector cmd)
{
	t_vector	paths;
	char		*cmd_path;
	size_t		i;

	i = 0;
	paths = stat_get()->paths;
	while (i < ft_vector_length(paths))
	{
		cmd_path = get_cmd_path(ft_vector_get(cmd, 0), ft_vector_get(paths, i));
		if (access(cmd_path, X_OK) == 0)
		{
			ft_vector_set(cmd, cmd_path, 0);
			execute(cmd);
		}
		free(cmd_path);
		++i;
	}
	ft_dprintf(STDERR_FILENO, "pipex: %s: command not found\n",
		(char *)ft_vector_get(cmd, 0));
	pipex_exit(127);
}

static int	execute_command(t_vector cmd, int pipefd[2], int index, int length)
{
	int	pid;
	int	ttyfd[2];
	int	redir_ret;

	ttyfd[0] = dup(STDIN_FILENO);
	ttyfd[1] = dup(STDOUT_FILENO);
	pipe(pipefd);
	redir_ret = make_redirections(index, length, ttyfd, pipefd);
	pid = fork();
	if (pid == 0)
	{
		close_two(ttyfd);
		close_pipes(pipefd - (index * 2), index + 1);
		if (redir_ret != 0)
			pipex_exit(2);
		if (access(ft_vector_get(cmd, 0), X_OK) == 0)
			execute(cmd);
		else
			execute_from_path(cmd);
		ft_gc_destroy(stat_get()->gc);
	}
	dup2(ttyfd[0], STDIN_FILENO);
	dup2(ttyfd[1], STDOUT_FILENO);
	close_two(ttyfd);
	return (pid);
}

/*
** Wait for child processes to terminate.
**
** The exit status of the last command of the pipeline is returned.
*/

static int	wait_for_children(int *pipefd, pid_t last_pid, int length)
{
	size_t	i;
	int		wstatus;
	int		last_status;
	pid_t	pid;

	i = 0;
	last_status = 0;
	while (1)
	{
		pid = wait(&wstatus);
		if (pid <= 0)
			break ;
		if (i++ == 0)
			close_pipes(pipefd, length);
		if (pid == last_pid)
			last_status = WEXITSTATUS(wstatus);
	}
	return (last_status);
}

int	execute_pipeline(t_vector pipeline)
{
	size_t		i;
	t_vector	cmd;
	int			*pipefd;
	size_t		length;
	pid_t		last_pid;

	length = ft_vector_length(pipeline);
	pipefd = ft_gc_add(stat_get()->gc, assert_ptr(malloc(sizeof (int)
					* length * 2)), &free);
	i = 0;
	while (i < ft_vector_length(pipeline))
	{
		cmd = ft_vector_get(pipeline, i);
		last_pid = execute_command(cmd, pipefd + (i * 2), i,
				ft_vector_length(pipeline));
		++i;
	}
	return (wait_for_children(pipefd, last_pid, length));
}
