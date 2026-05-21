/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 17:02:26 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/21 16:27:16 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/types/struct_sched_param.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sched.h>

#include "modules/mbx_constants.h"
#include "modules/mbx_handlers.h"
#include "modules/mbx_scancodes.h"
#include "tsr.h"
#include "tsr_context.h"

static void	init_player(t_tsr *tsr)
{
	tsr->player.position = vec3(8.0, 8.0, 4.0);
	tsr->player.rotation = vec3(M_PI, 0.001, 0.001);
	tsr->camera.rotation = vec3(M_PI, 0.001, 0.001);
	tsr->player.hitbox = vec3(0.4, 0.9, 0.4);
	tsr->player.velocity = vec3_zero();
	tsr->player.tile_id = 1;
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
	tsr->mbx->settings.exit_key = MBX_KEY_NONE;
	tsr->mbx->settings.viewport_render = MBX_VIEWPORT_RENDER_SKIP;
	load_tile_data(tsr);
	atlas_init(&tsr->atlas);
	init_textures(tsr);
	init_player(tsr);
	tsr->world.global_light = vec3(0.0, 0.5, 0.866025);
	tsr->world.global_light_col = vec4(1.8, 1.2, 0.8, 1.0);
	if (!mbx_make_main_window(tsr->mbx,
			vec2i(DEFAULT_VIEWPORT_W, DEFAULT_VIEWPORT_H),
			DEFAULT_WINDOW_TITLE, DEFAULT_WINDOW_FLAGS))
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLMBXWIN);
	tsr->rendering.target = mbx_make_region_with_image(
			tsr->mbx, tsr->mbx->vp->size);
	tsr->rendering.swap_target = tsr->mbx->viewport;
	tsr->ui.state = UI_STATE_MAIN;
	tsr->ui.target = mbx_make_region(tsr->mbx, tsr->mbx->vp->size);
	tsr->ui.hotbar.offset = 1;
	if (!tsr->rendering.target || !tsr->ui.target)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	init_threads(tsr);
	tsr->extras.aspect_ratio = (double)DEFAULT_VIEWPORT_H / DEFAULT_VIEWPORT_W;
	tsr->extras.fov = 90.0;
	tsr->extras.focal_length = 1.0;
	return (tsr);
}
