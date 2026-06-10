/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 02:22:48 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/27 18:00:43 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr_utils.h"

size_t	tsr_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i += 1;
	return (i);
}

int	tsr_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*tsr_strcat(char *dst, const char *src)
{
	const size_t	i = tsr_strlen(dst);
	size_t			j;

	j = tsr_strlen(src);
	dst[i + j] = 0;
	while (j--)
		dst[i + j] = src[j];
	return (dst);
}

char	*tsr_strdup(const char *src)
{
	char	*dst;
	size_t	len;

	len = tsr_strlen(src);
	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	dst[len] = 0;
	while (len--)
		dst[len] = src[len];
	return (dst);
}

char	tsr_tolower(char c)
{
	if (c >= 'A' && c < 'Z')
		c += 32;
	return (c);
}
