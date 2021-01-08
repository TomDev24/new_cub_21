/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/08 14:50:11 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/20 16:53:02 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	size_t	len;
	char	*res;

	if (!s1 || !s2)
		return (0);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	len = len_s1 + len_s2 + 1;
	res = (char *)malloc(sizeof(char) * len);
	if (res)
	{
		ft_memcpy(res, s1, len_s1);
		ft_memcpy(res + len_s1, s2, len_s2);
		*(res + len_s1 + len_s2) = '\0';
	}
	else
		errno = ENOMEM;
	return (res);
}

int		free_p(void **p1, void **p2, void *p3, int *bytes)
{
	if (p1 && *p1)
	{
		free(*p1);
		*p1 = NULL;
	}
	if (p2 && *p2)
	{
		free(*p2);
		*p2 = NULL;
	}
	if (p3)
	{
		free(p3);
		p3 = NULL;
	}
	return (*bytes);
}

int		what_left(char **left, char **line, char **buffer, int *bytes)
{
	char *n_point;
	char *tmp;

	if (!*left)
		return (0);
	if ((n_point = ft_strchr(*left, '\n')))
	{
		*n_point = '\0';
		tmp = *line;
		*line = ft_strjoin(*line, *left);
		free(tmp);
		tmp = *left;
		*left = ft_strdup(++n_point);
		free_p((void**)&tmp, (void**)buffer, NULL, bytes);
		return (*bytes > 0 ? 1 : 0);
	}
	else
	{
		tmp = *line;
		*line = ft_strjoin(*line, *left);
		free_p((void**)&tmp, (void**)left, NULL, bytes);
	}
	return (0);
}

int		init(char **line, char **buffer, char **left, int *bytes)
{
	if (!line || BUFFER_SIZE < 1 || !(*line = ft_strdup("")))
		return (-1);
	if (!(*buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE + 1)))
		return (-1);
	if (what_left(left, line, buffer, bytes))
		return (*bytes > 0 ? 1 : 0);
	return (0);
}

int		get_next_line(int fd, char **line)
{
	static char	*left;
	char		*buffer;
	static int	bytes;
	char		*n_point;
	char		*tmp;

	bytes = bytes ? bytes : 0;
	if ((bytes = init(line, &buffer, &left, &bytes)))
		return (bytes);
	while ((bytes = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes] = '\0';
		if ((n_point = ft_strchr(buffer, '\n')))
		{
			*n_point = '\0';
			left = ft_strdup(++n_point);
			tmp = *line;
			*line = ft_strjoin(*line, buffer);
			return (free_p((void**)&tmp, (void**)&buffer, NULL, &bytes));
		}
		tmp = *line;
		*line = ft_strjoin(*line, buffer);
		free(tmp);
	}
	return (buffer ? free_p(NULL, (void**)&buffer, NULL, &bytes) : bytes);
}
