/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 12:19:55 by dbrittan          #+#    #+#             */
/*   Updated: 2020/11/20 16:56:45 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	if (*((char*)s) == '\0')
		return (0);
	while (*s)
	{
		s++;
		i++;
	}
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t i;

	i = -1;
	if (!dst || !src)
		return (0);
	while (dstsize != 0 && ++i < dstsize - 1 && src[i])
		dst[i] = src[i];
	if (dstsize != 0)
		dst[i] = '\0';
	return (ft_strlen(src));
}

char	*ft_strdup(const char *s1)
{
	char	*res;
	size_t	len;

	len = ft_strlen(s1);
	res = (char*)malloc(sizeof(*s1) * (len + 1));
	if (res)
		ft_strlcpy(res, s1, sizeof(*s1) * (len + 1));
	else
		errno = ENOMEM;
	return (res);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s != '\0')
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*a;
	unsigned char	*b;

	a = (unsigned char *)dst;
	b = (unsigned char *)src;
	i = -1;
	if (!dst || !src)
		return (dst);
	while (++i < n)
		a[i] = b[i];
	return (dst);
}
