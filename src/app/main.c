/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:27:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 16:55:39 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_constants.h"
#include "modules/mbx_drawing.h"
#include "modules/mbx_handlers.h"
#include "modules/mbx_inputs.h"
#include "modules/mbx_utils.h"
#include "tsr.h"
#include "tsr_constants.h"
#include <stdatomic.h>
#include <stdio.h>

static void	world_build(t_world *world)
{
	for (int x = 0; x < world->size.x; x++)
		for (int y = 0; y < world->size.y; y++)
			for (int z = 0; z < world->size.z; z++)
				block_set(world, vec3i(x, y, z), (y == 0 || y == 15)
					|| ((5 <= x && x <= 10 && 5 <= y && y <= 10 && 5 <= z && z <= 10)
						&& !(((6 <= x && x <= 9) && (6 <= y && y <= 9))
							|| ((6 <= y && y <= 9) && (6 <= z && z <= 9))
							|| ((6 <= x && x <= 9) && (6 <= z && z <= 9))))
					);
	block_set(world, vec3i(0, 0, 0), 3);
}

void	update(t_mbx *mbx, void *data)
{
	t_tsr	*tsr;
	char	str[256];
	double	now;

	(void)mbx;
	tsr = data;
	tsr_update_camera(tsr);
	tsr_update_player(tsr);
	if (mbx_btnp(tsr->mbx, MBX_KEY_F1))
	{
		tsr->mbx->settings.lock_cursor = !tsr->mbx->settings.lock_cursor;
		tsr->mbx->settings.show_cursor = !tsr->mbx->settings.show_cursor;
		mbx_refresh_settings(tsr->mbx);
	}
	now = mbx_get_timestamp();
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	tsr->rendering.current_job = 0;
	tsr->rendering.job_region_count = vec2i(
			tsr->mbx->vp->size.x / RENDER_JOB_REGION_W,
			tsr->mbx->vp->size.y / RENDER_JOB_REGION_H);
	tsr->rendering.job_count = (tsr->rendering.job_region_count.x
			* tsr->rendering.job_region_count.y);
	snprintf(str, 256, "FPS \t%.2f\nPOS \t[%.2f %.2f %.2f]\n"
		"ROT \t[%.2f %.2f]\n",
		1.0 / tsr->mbx->dt, tsr->player.position.x, tsr->player.position.y,
		tsr->player.position.z,
		tsr->camera.rotation.x, tsr->camera.rotation.y);
	mbx_set_text_scaled(tsr->mbx->vp, str,
		vec2ix2_xy(5, 5, 2, 2), tsr->ui.fonts.small);
	mbx_render_region_as_viewport(tsr->mbx, tsr->mbx->vp,
		MBX_VIEWPORT_RENDER_KEEP);
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
}

int	main(void)
{
	t_tsr	*tsr;

	tsr_report(STATUS_DEBUG, REPORT_DEBUG_ON);
	tsr = tsr_init();
	world_create(&tsr->wworld, vec3i_i(16));
	world_build(&tsr->wworld);
	tsr->mbx->settings.fps_cap = 240;
	mbx_run(tsr->mbx, update, tsr);
	tsr_exit(tsr, STATUS_INFO, REPORT_SUCCESS);
}
