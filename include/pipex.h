#ifndef PIPEX_H
# define PIPEX_H
# include <stddef.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strtok(char *str, const char *delim);
int		ft_strncmp(const char *s1, const char *s2, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);
#endif
