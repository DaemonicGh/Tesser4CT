/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 19:15:43 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/29 01:27:39 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include <pthread.h>

static void	cancel_threads(t_tsr *tsr)
{
	size_t	i;

	if (!tsr->rendering.running)
		return ;
	tsr->rendering.running = false;
	if (tsr->rendering.threads)
	{
		i = 0;
		pthread_barrier_wait(&tsr->rendering.wait_barrier);
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
	free(tsr->wworld.blocks);
	mlem_destroy(tsr->textures.textures_mlem);
	mlem_destroy(tsr->world.tiles_mlem);
	mlem_destroy(tsr->world.map_mlem);
	free(tsr->world.chunks);
	mbx_exit(tsr->mbx);
	free(tsr);
	exit(!tsr_report(status, message));
}
