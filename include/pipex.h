#ifndef PIPEX_H
# define PIPEX_H
# include <stddef.h>
# include "lib.h"

typedef struct s_stat
{
	t_argv	*paths;
	char	**envp;
}	t_stat;

typedef struct	s_pipeline
{
	size_t	cap;
	size_t	length;
	t_argv	**cmds;
}	t_pipeline;

t_pipeline	*pipeline_new(size_t cap);
void		pipeline_append(t_pipeline *pipeline, t_argv *cmd);
void		pipeline_destroy(t_pipeline *pipeline, void (*fn)(t_argv *cmd));

t_stat		*stat_get(void);

t_argv	*get_paths(char **env);
t_argv	*command_parse(char *arg);
#endif
