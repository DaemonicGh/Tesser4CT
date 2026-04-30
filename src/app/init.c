/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 17:02:26 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/30 15:52:26 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/types/struct_sched_param.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#include "tsr.h"

static void	init_player(t_tsr *tsr)
{
	tsr->player.position = vec3(8.0, 8.0, 4.0);
	tsr->player.rotation = vec3(M_PI, 0, 0);
	tsr->camera.rotation = vec3(M_PI, 0, 0);
	tsr->player.velocity = vec3_zero();
}

static void	init_threads(t_tsr *tsr)
{
	size_t				i;

	tsr->rendering.running = true;
	tsr->rendering.thread_count = max(sysconf(_SC_NPROCESSORS_ONLN) - 1, 1);
	tsr->rendering.threads = malloc(
			tsr->rendering.thread_count * sizeof(pthread_t));
	if (!tsr->rendering.threads)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	if (pthread_barrier_init(&tsr->rendering.wait_barrier, NULL,
			tsr->rendering.thread_count + 1))
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLTHREAD);
	i = 0;
	while (i < tsr->rendering.thread_count)
	{
		if (pthread_create(&tsr->rendering.threads[i],
				NULL, tsr_render_thread_init, tsr))
			tsr_exit(tsr, STATUS_ERROR, REPORT_NULLTHREAD);
		i++;
	}
	tsr->rendering.frag_shader = draw_ray;
}

void	init_tiles(t_tsr *tsr)
{
	size_t	i;

	i = 0;
	while (i < TILE_COUNT)
	{
		tsr->world.tiles[i] = g_tile_data[i];
		i++;
	}
}

t_tsr	*tsr_init(void)
{
	t_tsr	*tsr;

	tsr = malloc(sizeof(t_tsr));
	if (!tsr)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	*tsr = (t_tsr){0};
	tsr->mbx = mbx_init_windowless();
	if (!tsr->mbx)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLMBX);
	init_tiles(tsr);
	atlas_init(&tsr->atlas, tsr->mbx);
	init_textures(tsr);
	init_player(tsr);
	tsr->world.global_light = vec3_normalize(vec3(0.35, 1.0, 0.55));
	if (!mbx_make_main_window(tsr->mbx,
			vec2i(DEFAULT_VIEWPORT_W, DEFAULT_VIEWPORT_H),
			DEFAULT_WINDOW_TITLE, DEFAULT_WINDOW_FLAGS))
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLMBXWIN);
	init_threads(tsr);
	tsr->extras.aspect_ratio = (double)DEFAULT_VIEWPORT_H / DEFAULT_VIEWPORT_W;
	tsr->mbx->settings.show_cursor = false;
	tsr->mbx->settings.lock_cursor = true;
	tsr->mbx->settings.viewport_render = MBX_VIEWPORT_RENDER_SKIP;
	return (tsr);
}
