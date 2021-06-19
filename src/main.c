#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "pipex.h"
#include "lib.h"

char	*get_cmd_path(const char *cmd, const char *path)
{
	char	*cmd_path;
	size_t	i;

	cmd_path = malloc(sizeof (*cmd_path) + ft_strlen(cmd)
			+ ft_strlen(path) + 2);
	if (cmd_path == NULL)
		return (NULL);
	i = 0;
	while (*path != '\0')
		cmd_path[i++] = *path++;
	cmd_path[i++] = '/';
	while (*cmd != '\0')
		cmd_path[i++] = *cmd++;
	cmd_path[i] = '\0';
	return (cmd_path);
}

void	execute_command(t_argv *cmd, int *pipefd, int index, int length)
{
	t_argv	*paths;
	char	*cmd_path;
	int		pid;
	size_t	i;
	
	i = 0;
	paths = stat_get()->paths;
	pipe(pipefd);
	while (i < paths->length)
	{
		cmd_path = get_cmd_path(cmd->args[0], paths->args[i]);
		if (access(cmd_path, X_OK) == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				if (index != 0)
					dup2(pipefd[-2], STDIN_FILENO);
				if (index != length - 1)
					dup2(pipefd[1], STDOUT_FILENO);
				execve(cmd_path, cmd->args, stat_get()->envp);
			}
			close(pipefd[1]);
			free(cmd_path);
			waitpid(pid, NULL, 0);
			break ;
		}
		++i;
	}
}

void	execute_pipeline(t_pipeline *pipeline)
{
	int		*pipefd;
	size_t	i;

	pipefd = malloc(sizeof (int) * pipeline->length * 2);
	if (pipefd == NULL)
		return ;
	i = 0;
	while (i < pipeline->length)
	{
		execute_command(pipeline->cmds[i], pipefd + (i * 2), i, pipeline->length);
		++i;
	}
}


int	main(int argc, char **argv, char **envp)
{
	t_argv		*paths;
	t_pipeline	*pipeline;

	pipeline = pipeline_new(1);
	while (*(++argv) != NULL)
		pipeline_append(pipeline, command_parse(*argv));
	stat_get()->paths = get_paths(envp);
	stat_get()->envp = envp;
	execute_pipeline(pipeline);
	//exec_cmd(command_parse(argv[2]), paths, env, pipefd);
}
