#ifndef PIPEX_H
# define PIPEX_H
# include <stddef.h>
# include "libft/gc.h"
# include "libft/vector.h"

# define HERE_DOC_MODE_WORD "here_doc"
# define HERE_DOC_FILE ".here_doc"
# define HERE_DOC_BUFFER_SIZE 10000

# define PIPEX_USAGE "pipex [here_doc] <in_arg> <cmd> <cmd2> ... <cmdn> <out>\n"

enum e_mode {
	MODE_NORMAL,
	MODE_HERE_DOC,
};

typedef struct s_stat
{
	t_gc		gc;
	t_vector	paths;
	char		**envp;
	enum e_mode	mode;
	char		*in_arg;
	char		*out_arg;
}	t_stat;

t_vector	parsecl(int argc, char **argv);

t_stat		*stat_get(void);

t_vector	get_paths(char **env);
char		*get_cmd_path(const char *cmd, const char *path);

int			execute_pipeline(t_vector pipeline);

int			make_redirections(int index, int length, int ttyfd[2],
				int pipefd[2]);
int			here_doc(const char *delim);

void		close_pipes(int *pipefd, int length);
void		close_two(int fd[2]);
t_vector	command_parse(char *arg);
void		command_destroy(t_vector cmd);
void		pipeline_destroy(t_vector pipeline);

void		*assert_ptr(void *p);
void		pipex_exit(int exit_code);

#endif
