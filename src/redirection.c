#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>

#include "libft/io.h"

#include "pipex.h"

static int	handle_redir_out(void)
{
	const char	*filepath;
	int			fd;

	filepath = stat_get()->out_arg;
	if (stat_get()->mode == MODE_HERE_DOC)
		fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "pipex: %s: %s\n", filepath, strerror(errno));
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_in(int ttyfd[2])
{
	const char	*arg;
	int			fd;
	int			save_out;

	arg = stat_get()->in_arg;
	if (stat_get()->mode == MODE_HERE_DOC)
		fd = here_doc(arg);
	else
		fd = open(arg, O_RDONLY);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "pipex: %s: %s\n", arg, strerror(errno));
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	make_redirections(int index, int length, int ttyfd[2], int pipefd[2])
{
	int	ret;

	ret = 0;
	if (index > 0)
		dup2(pipefd[-2], STDIN_FILENO);
	else
		ret = handle_redir_in(ttyfd);
	if (index == length - 1)
		ret = handle_redir_out();
	else
		dup2(pipefd[1], STDOUT_FILENO);
	return (ret);
}
