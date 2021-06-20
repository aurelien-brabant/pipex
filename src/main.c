#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>

#include "pipex.h"
#include "lib.h"




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

void	execute_pipeline(t_pipeline *pipeline)
{
	int		*pipefd;
	int		*cur_pipefd;
	size_t	i;

	pipefd = malloc(sizeof (int) * pipeline->length * 2);
	if (pipefd == NULL)
		return ;
	i = 0;
	while (i < pipeline->length)
	{
		cur_pipefd = pipefd + (i * 2);
		pipe(cur_pipefd);
		execute_command(pipeline, cur_pipefd, i, pipeline->length);
		close(cur_pipefd[1]);
		++i;
	}
}


int	main(int argc, char **argv, char **envp)
{
	t_argv		*paths;
	t_pipeline	*pipeline;
	size_t		i;

	if (argc < 5)
		return (1);
	i = 1;
	pipeline = pipeline_new(10);
	if (strcmp(argv[i], "here_doc") == 0)
	{
		if (argc < 6)
		{
			dprintf(STDERR_FILENO, "Missing outfile for here_doc mode\n");
			return (3);
		}
		stat_get()->mode = MODE_HERE_DOC;
		++i;
	}
	stat_get()->filein = argv[i++];
	while (i < argc - 1)
		pipeline_append(pipeline, command_parse(argv[i++]));
	stat_get()->fileout = argv[i];
	stat_get()->paths = get_paths(envp);
	stat_get()->envp = envp;
	execute_pipeline(pipeline);
}
