#ifndef PIPEX_H
# define PIPEX_H
# include <stddef.h>
# include "lib.h"

enum e_mode {
	MODE_NORMAL,
	MODE_HERE_DOC,
};

typedef struct s_stat
{
	t_argv		*paths;
	char		**envp;
	enum e_mode	mode;
	char		*filein;
	char		*fileout;
}	t_stat;

typedef struct	s_pipeline
{
	size_t	cap;
	size_t	length;
	t_argv	**cmds;
	char	*filein;
	char	*fileout;
}	t_pipeline;

t_pipeline	*pipeline_new(size_t cap);
void		pipeline_append(t_pipeline *pipeline, t_argv *cmd);
void		pipeline_destroy(t_pipeline *pipeline, void (*fn)(t_argv *cmd));
void		write_until_delim(int pipefd[2], const char *delim);

t_stat		*stat_get(void);

t_argv	*get_paths(char **env);
char	*get_cmd_path(const char *cmd, const char *path);
t_argv	*command_parse(char *arg);


void	exec_normal(t_pipeline *pipeline, char *cmd_path, int *pipefd,
		int index);
void	exec_here_doc(t_pipeline *pipeline, char *cmd_path, int *pipefd,
			int index);
#endif
