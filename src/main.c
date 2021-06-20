#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>

#include "libft/gc.h"
#include "libft/vector.h"

#include "pipex.h"

/*
void	execute_command(t_pipeline *pipeline, int *pipefd, int index, int length)
{
	t_argv	*paths;
	char	*cmd_path;
	int		pid;
	size_t	i;
	char	*cmd;
	
	cmd = pipeline->cmds[index]->args[0];
	i = 0;
	paths = stat_get()->paths;
	while (i < paths->length)
	{
		cmd_path = get_cmd_path(cmd, paths->args[i]);
		if (access(cmd_path, X_OK) == 0)
		{
			if (stat_get()->mode == MODE_NORMAL)
				exec_normal(pipeline, cmd_path, pipefd, index);
			else if (stat_get()->mode == MODE_HERE_DOC)
				exec_here_doc(pipeline, cmd_path, pipefd, index);
			return ;
		}
		++i;
	}
	dprintf(STDERR_FILENO, "pipex: %s: command not found\n", cmd);
}
*/

void	execute_normal(t_argv *cmd, int *pipefd, int index, int length)
{
	int	pid;
	int	fd[2];

	fd[0] = open(stat_get()->in_arg, O_RDONLY);
	fd[1] = open(stat_get()->out_arg, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	pid = fork();
	if (pid == 0)
	{
		if (index == 0)
		execve(cmd->args[0], cmd->args, stat_get()->envp);
	}
	waitpid(pid, NULL, 0);
}

void	execute_pipeline(t_vector pipeline, int *pipefd)
{
	size_t	i;
	t_argv	*cmd;

	i = 0;
	while (i < ft_vector_length(pipeline))
	{
		cmd = ft_vector_get(pipeline, i);
		if (access(cmd->args[0], X_OK) == 0)
			execute(cmd, pipefd, i, ft_vector_length(pipeline));
		++i;
	}
}

int	main(int argc, char **argv, char *envp[])
{
	t_gc		gc;
	t_vector	pipeline;
	int			*pipefd;

	gc = ft_gc_new();
	if (gc == NULL)
		return (1);
	stat_get()->gc = gc;
	stat_get()->envp = envp;
	pipeline = parsecl(argc, argv);
	stat_get()->paths = get_paths(envp);
	pipefd = ft_gc_add(stat_get()->gc, assert_ptr(
				malloc(sizeof (int) * ft_vector_length(pipeline) * 2)), &free);
	execute_pipeline(pipeline, pipefd);
	return (0);
}
