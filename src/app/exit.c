/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 19:15:43 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/13 18:33:35 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_handlers.h"
#include "tsr.h"
#include "tsr_context.h"
#include "tsr_core.h"
#include <pthread.h>

static void	unload_textures(t_tsr *tsr)
{
	size_t			i;
	t_tsr_texture	*texture;

	i = 0;
	while (i < ATLAS_LENGTH)
	{
		texture = &tsr->atlas.texture[i];
		if (texture)
		{
			free(tsr->atlas.key[i]);
			tsr->atlas.key[i] = NULL;
			mbx_destroy_region(tsr->mbx, texture->tx);
			if (texture->nrm)
				mbx_destroy_region(tsr->mbx, texture->nrm);
		}
		i++;
	}
	i = 0;
	while (i < TSR_TEXTURE_COUNT)
	{
		if (tsr->textures.regions[i] != tsr->textures._default)
			mbx_destroy_region(tsr->mbx, tsr->textures.regions[i]);
		i++;
	}
	mbx_destroy_region(tsr->mbx, tsr->textures._default);
	mbx_destroy_region(tsr->mbx, tsr->nmap);
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
	free(tsr->wworld.blocks);
	mbx_destroy_region(tsr->mbx, tsr->rendering.target);
	mbx_destroy_region(tsr->mbx, tsr->rendering.swap_target);
	tsr->mbx->vp = NULL;
	mbx_destroy_region(tsr->mbx, tsr->ui.target);
	if (tsr->mbx)
		mbx_exit(tsr->mbx);
	free(tsr);
	exit(!tsr_report(status, message));
}
