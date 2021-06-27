#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "libft/io.h"
#include "libft/cstring.h"

#include "pipex.h"


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

int	here_doc(const char *delim)
{
	char	line[HERE_DOC_BUFFER_SIZE];
	size_t	lc;
	int		ret;
	int		fd[2];

	fd[1] = open(HERE_DOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	fd[0] = open(HERE_DOC_FILE, O_RDONLY);
	lc = 1;
	ret = get_line(line);
	while (ret > 0 && ft_strcmp(line, delim) != 0)
	{
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		++lc;
		ret = get_line(line);
	}
	close(fd[1]);
	if (ret == 0)
		dprintf(STDERR_FILENO, "\npipex: warning: here-document at line %ld"
			" delimited by end-of-file (wanted '%s')\n", lc, delim);
	return (fd[0]);
}
