#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "libft/io.h"

#include "pipex.h"

static int	open_fds(int *fd, int here_doc_pipefd[2], int index, int length)
{
	char	*out;

	out = stat_get()->out_arg;
	if (index == 0)
	{
		pipe(here_doc_pipefd);
		here_doc(here_doc_pipefd, stat_get()->in_arg);
	}
	if (index == length - 1)
	{
		*fd = open(out, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*fd == -1)
		{
			ft_dprintf(STDERR_FILENO, "pipex: %s: %s\n", out, strerror(errno));
			return (1);
		}
	}
	return (0);
}

static void	close_fds(int fd, int here_doc_pipefd[2], int index, int length)
{
	if (index == 0)
		close(here_doc_pipefd[0]);
	else if (index == length - 1)
		close(fd);
}

void	execute_here_doc(t_argv *cmd, int *pipefd, int index, int length)
{
	int	pid;
	int	fd;
	int	here_doc_pipefd[2];

	if (open_fds(&fd, here_doc_pipefd, index, length) != 0)
	{
		close_fds(fd, here_doc_pipefd, index, length);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (index == 0)
			dup2(here_doc_pipefd[0], STDIN_FILENO);
		else
			dup2(pipefd[-2], STDIN_FILENO);
		if (index == length - 1)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(pipefd[1], STDOUT_FILENO);
		execve(cmd->args[0], cmd->args, stat_get()->envp);
	}
	waitpid(pid, NULL, 0);
	close_fds(fd, here_doc_pipefd, index, length);
}
