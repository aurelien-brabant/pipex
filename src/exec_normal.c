#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "libft/io.h"

#include "pipex.h"

static int	open_fds(int fd[2], int index, int length)
{
	char	*in;
	char	*out;

	in = stat_get()->in_arg;
	out = stat_get()->out_arg;
	if (index == 0)
	{
		fd[0] = open(in, O_RDONLY);
		if (fd[0] == -1)
		{
			ft_dprintf(STDERR_FILENO, "pipex: %s: %s", in, strerror(errno));
			return (1);
		}
	}
	if (index == length - 1)
	{
		fd[1] = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd[1] == -1)
		{
			ft_dprintf(STDERR_FILENO, "pipex: %s: %s\n", out, strerror(errno));
			return (1);
		}
	}
	return (0);
}

static void	close_fds(int fd[2], int index, int length)
{
	if (index == 0)
		close(fd[0]);
	else if (index == length - 1)
		close(fd[1]);
}

void	execute_normal(t_argv *cmd, int *pipefd, int index, int length)
{
	int	pid;
	int	fd[2];

	if (open_fds(fd, index, length) != 0)
		return ;
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		if (index == 0)
			dup2(fd[0], STDIN_FILENO);
		else
			dup2(pipefd[-2], STDIN_FILENO);
		if (index == length - 1)
			dup2(fd[1], STDOUT_FILENO);
		else
			dup2(pipefd[1], STDOUT_FILENO);
		execve(cmd->args[0], cmd->args, stat_get()->envp);
	}
	waitpid(pid, NULL, 0);
	close(pipefd[1]);
	close_fds(fd, index, length);
}
