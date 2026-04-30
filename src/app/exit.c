/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 19:15:43 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/30 15:04:35 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"
#include <pthread.h>

static void	unload_textures(t_tsr *tsr)
{
	size_t			i;
	t_mbx_region	*region;

	i = 0;
	while (i < ATLAS_LENGTH)
	{
		region = atlas_index(&tsr->atlas, i);
		if (region)
			mbx_destroy_region(tsr->mbx, region);
		i++;
	}
	if (tsr->ui.fonts.small != tsr->extras.default_region)
		mbx_destroy_region(tsr->mbx, tsr->ui.fonts.small);
	mbx_destroy_region(tsr->mbx, tsr->extras.default_region);
}

static void	cancel_threads(t_tsr *tsr)
{
	size_t	i;

	tsr->rendering.running = false;
	if (tsr->rendering.threads)
	{
		pthread_barrier_wait(&tsr->rendering.wait_barrier);
		i = 0;
		while (i < tsr->rendering.thread_count
			&& tsr->rendering.threads[i])
			pthread_join(tsr->rendering.threads[i++], NULL);
		free(tsr->rendering.threads);
	}
	pthread_barrier_destroy(&tsr->rendering.wait_barrier);
}

void	tsr_exit(t_tsr *tsr, t_tsr_status status, const char *message)
{
	if (!tsr)
		exit(!tsr_report(status, message));
	cancel_threads(tsr);
	unload_textures(tsr);
	if (tsr->wworld.blocks)
		free(tsr->wworld.blocks);
	if (tsr->mbx)
		mbx_exit(tsr->mbx);
	free(tsr);
	exit(!tsr_report(status, message));
}
