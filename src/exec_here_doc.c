#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipex.h"

void	exec_here_doc(t_pipeline *pipeline, char *cmd_path, int *pipefd,
			int index)
{
	int	here_doc_pipefd[2];
	int	pid;
	int	fd;

	if (index == 0)
	{
		pipe(here_doc_pipefd);
		write_until_delim(here_doc_pipefd, stat_get()->filein);
	}
	else if (index == pipeline->length - 1)
	{
		fd = open(stat_get()->fileout, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
		{
			dprintf(STDERR_FILENO, "pipex: %s: %s\n",
					stat_get()->fileout, strerror(errno));
			return ;
		}
	}
	pid = fork();
	if (pid == 0)
	{
		if (index == 0)
			dup2(here_doc_pipefd[0], STDIN_FILENO);
		else
			dup2(pipefd[-2], STDIN_FILENO);
		if (index == pipeline->length - 1)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(pipefd[1], STDOUT_FILENO);
		execve(cmd_path, pipeline->cmds[index]->args, stat_get()->envp);
	}
	if (index == 0)
		close(here_doc_pipefd[0]);
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
