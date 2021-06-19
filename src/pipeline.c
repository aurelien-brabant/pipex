#include <stdlib.h>

#include "pipex.h"

static void	resize(t_pipeline *pipeline)
{
	t_argv	**cmds;

	cmds = ft_calloc(pipeline->cap * 2, sizeof (t_argv *));
	if (cmds == NULL)
		return ;
	ft_memcpy(cmds, pipeline->cmds, sizeof (t_argv *) * pipeline->length);
	free(pipeline->cmds);
	pipeline->cmds = cmds;
	pipeline->cap *= 2;
}

t_pipeline	*pipeline_new(size_t cap)
{
	t_pipeline	*pipeline;

	pipeline = ft_calloc(1, sizeof (*pipeline));
	if (pipeline == NULL)
		return (NULL);
	pipeline->cmds = ft_calloc(cap, sizeof (t_argv *));
	if (pipeline->cmds == NULL)
		return (NULL);
	pipeline->cap = cap;
	return (pipeline);
}

void	pipeline_append(t_pipeline *pipeline, t_argv *cmd)
{
	if (pipeline->length == pipeline->cap)
		resize(pipeline);
	pipeline->cmds[pipeline->length++] = cmd;
}

void	pipeline_destroy(t_pipeline *pipeline, void (*fn)(t_argv *cmd))
{
	size_t	i;

	i = 0;
	if (fn != NULL)
	{
		while (i < pipeline->length)
			fn(pipeline->cmds[i++]);
	}
	free(pipeline->cmds);
	free(pipeline);
}
