/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:21:34 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/09 09:57:37 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr_utils.h"

void	*tsr_batch_free(size_t ptr_count, ...)
{
	va_list		va;

	va_start(va, ptr_count);
	while (ptr_count--)
		free(va_arg(va, void *));
	va_end(va);
	return (NULL);
}

void	*tsr_memreport(void *pointer)
{
	if (pointer)
		return (pointer);
	tsr_report(STATUS_ERROR, REPORT_MEMORY);
	return (NULL);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i += 1;
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strcat(char *dst, const char *src)
{
	const size_t	i = ft_strlen(dst);
	size_t			j;

	j = ft_strlen(src);
	dst[i + j] = 0;
	while (j--)
		dst[i + j] = src[j];
	return (dst);
}
