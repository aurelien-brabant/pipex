#ifndef LIB_H
# define LIB_H
# include <stddef.h>

typedef struct s_argv
{
	size_t	length;
	size_t	cap;
	char	**args;
}	t_argv;

t_argv	*argv_new(size_t size);
void	argv_remove(t_argv *argv, size_t index);
void	argv_append(t_argv *argv, char *arg);
void	argv_destroy(t_argv *argv, void (*fn)(char *arg));

size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strtok(char *str, const char *delim);
int		ft_strncmp(const char *s1, const char *s2, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);

#endif
