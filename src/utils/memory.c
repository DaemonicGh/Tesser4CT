/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:21:34 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 02:23:19 by rprieur          ###   ########.fr       */
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
