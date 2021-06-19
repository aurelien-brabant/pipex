#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

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

void	execute_command(t_pipeline *pipeline, int *pipefd, int index, int length)
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
		cmd_path = get_cmd_path(pipeline->cmds[index]->args[0], paths->args[i]);
		if (access(cmd_path, X_OK) == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				if (index == 0)
					dup2(pipeline->redir_in_fd, STDIN_FILENO);
				else
					dup2(pipefd[-2], STDIN_FILENO);
				if (index == length - 1)
					dup2(pipeline->redir_out_fd, STDOUT_FILENO);
				else
					dup2(pipefd[1], STDOUT_FILENO);
				execve(cmd_path, pipeline->cmds[index]->args, stat_get()->envp);
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
		execute_command(pipeline, pipefd + (i * 2), i, pipeline->length);
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
	pipeline = pipeline_new(10);
	pipeline->redir_in_fd = open(argv[1], O_RDONLY);
	if (pipeline->redir_in_fd == -1)
		perror("pipex: input file");
	i = 2;
	while (i < argc - 1)
		pipeline_append(pipeline, command_parse(argv[i++]));
	pipeline->redir_out_fd = open(argv[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (pipeline->redir_out_fd == -1)
		perror("pipex: output file");
	if (pipeline->redir_in_fd == -1 || pipeline->redir_out_fd == -1)
		return (2);
	stat_get()->paths = get_paths(envp);
	stat_get()->envp = envp;
	execute_pipeline(pipeline);
}
