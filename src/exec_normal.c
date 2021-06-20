#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipex.h"

static int	get_redir_fd(int *fd, int index, int pipeline_length)
{
	if (index == 0)
	{
		*fd = open(stat_get()->in_arg, O_RDONLY);
		if (*fd == -1)
		{
			dprintf(STDERR_FILENO, "pipex: %s: %s\n", 
					stat_get()->in_arg, strerror(errno));
			return (1);
		}
	}
	else if (index == pipeline_length - 1)
	{
		*fd = open(stat_get()->out_arg, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (*fd == -1)
		{
			dprintf(STDERR_FILENO, "pipex: %s: %s\n",
					stat_get()->out_arg, strerror(errno));
			return (1);
		}
	}
	return (0);
}

void	exec_normal(t_pipeline *pipeline, char *cmd_path, int *pipefd,
			int index)
{
	int	pid;
	int	fd;

	if (get_redir_fd(&fd, index, pipeline->length) != 0)
		return ;
	pid = fork();
	if (pid == 0)
	{
		if (index == 0)
			dup2(fd, STDIN_FILENO);
		else
			dup2(pipefd[-2], STDIN_FILENO);
		if (index == pipeline->length - 1)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(pipefd[1], STDOUT_FILENO);
		execve(cmd_path, pipeline->cmds[index]->args, stat_get()->envp);
	}
	close(pipefd[1]);
	pipefd[1] = -1;
	if (index > 0)
	{
		close(pipefd[-2]);
		pipefd[-2] = -1;
	}
	free(cmd_path);
	waitpid(pid, NULL, 0);
}
