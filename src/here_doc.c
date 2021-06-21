#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "libft/io.h"
#include "libft/cstring.h"

#include "pipex.h"

/*
** Handling of the here document feature (<<)
*/

static int	get_line(char *line)
{
	char	buf;
	int		ret;
	size_t	i;

	i = 0;
	write(STDOUT_FILENO, "> ", 2);
	ret = read(STDIN_FILENO, &buf, 1);
	while (ret > 0 && buf != '\n')
	{
		if (i + 1 >= HERE_DOC_BUFFER_SIZE)
		{
			ft_dprintf(STDERR_FILENO, "here_doc buffer size exceeded (%lld)\n",
				HERE_DOC_BUFFER_SIZE);
			line[i] = '\0';
			return (-1);
		}
		line[i++] = buf;
		ret = read(STDIN_FILENO, &buf, 1);
	}
	line[i] = '\0';
	return (ret);
}

void	write_until_delim(int pipefd[2], const char *delim)
{
	char	line[HERE_DOC_BUFFER_SIZE];
	size_t	lc;
	int		ret;

	lc = 1;
	ret = get_line(line);
	while (ret > 0 && ft_strcmp(line, delim) != 0)
	{
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		++lc;
		ret = get_line(line);
	}
	close(pipefd[1]);
	if (ret == 0)
		dprintf(STDERR_FILENO, "\npipex: warning: here-document at line %ld"
			" delimited by end-of-file (wanted '%s')\n", lc, delim);
}
