/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 19:15:43 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/08 18:05:36 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_handlers.h"
#include "tsr.h"
#include "tsr_core.h"
#include <pthread.h>

static void	unload_textures(t_tsr *tsr)
{
	size_t	i;
	t_pbr	*pbr;

	i = 0;
	while (i < ATLAS_LENGTH)
	{
		pbr = atlas_index(&tsr->atlas, i);
		if (pbr)
		{
			mbx_destroy_region(tsr->mbx, pbr->col_tex);
			if (pbr->nrm_tex)
				mbx_destroy_region(tsr->mbx, pbr->nrm_tex);
		}
		i++;
	}
	if (tsr->ui.fonts.small != tsr->extras.default_region)
		mbx_destroy_region(tsr->mbx, tsr->ui.fonts.small);
	if (tsr->ui.gui.hotbar != tsr->extras.default_region)
		mbx_destroy_region(tsr->mbx, tsr->ui.gui.hotbar);
	if (tsr->ui.gui.hotbar_selection != tsr->extras.default_region)
		mbx_destroy_region(tsr->mbx, tsr->ui.gui.hotbar_selection);
	mbx_destroy_region(tsr->mbx, tsr->extras.default_region);
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
	if (tsr->mbx)
		mbx_exit(tsr->mbx);
	free(tsr);
	exit(!tsr_report(status, message));
}
