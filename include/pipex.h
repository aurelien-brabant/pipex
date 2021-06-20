#ifndef PIPEX_H
# define PIPEX_H
# include <stddef.h>
# include "libft/gc.h"
# include "libft/vector.h"
# define HERE_DOC_MODE_WORD "here_doc"

enum e_mode {
	MODE_NORMAL,
	MODE_HERE_DOC,
};

typedef struct s_argv
{
	size_t	length;
	size_t	cap;
	char	**args;
}	t_argv;

typedef struct s_stat
{
	t_gc		gc;
	t_argv		*paths;
	char		**envp;
	enum e_mode	mode;
	char		*in_arg;
	char		*out_arg;
}	t_stat;

typedef struct	s_pipeline
{
	size_t	cap;
	size_t	length;
	t_argv	**cmds;
}	t_pipeline;


t_vector	parsecl(int argc, char **argv);

t_argv		*argv_new(size_t size);
void		argv_remove(t_argv *argv, size_t index);
void		argv_append(t_argv *argv, char *arg);
void		argv_destroy(t_argv *argv);

void		pipex_exit(int exit_code);

void		write_until_delim(int pipefd[2], const char *delim);

t_stat		*stat_get(void);

t_argv	*get_paths(char **env);
char	*get_cmd_path(const char *cmd, const char *path);
t_argv	*command_parse(char *arg);


/* PARSE COMMAND LINE */


void	exec_normal(t_pipeline *pipeline, char *cmd_path, int *pipefd,
		int index);
void	exec_here_doc(t_pipeline *pipeline, char *cmd_path, int *pipefd,
			int index);


void	*assert_ptr(void *p);
#endif
