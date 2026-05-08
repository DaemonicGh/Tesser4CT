/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:27:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/08 16:28:50 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include <stdatomic.h>
#include <stdio.h>

static void	world_build(t_world *world)
{
	for (int x = 0; x < world->size.x; x++)
		for (int y = 0; y < world->size.y; y++)
			for (int z = 0; z < world->size.z; z++)
				block_set(world, vec3i(x, y, z), y == 15
					|| ((5 <= x && x <= 10 && 5 <= y && y <= 10 && 5 <= z && z <= 10)
						&& !(((6 <= x && x <= 9) && (6 <= y && y <= 9))
							|| ((6 <= y && y <= 9) && (6 <= z && z <= 9))
							|| ((6 <= x && x <= 9) && (6 <= z && z <= 9))))
					);
	for (int x = 0; x < world->size.x; x++)
		for (int z = 0; z < world->size.z; z++)
			block_set(world, vec3i(x, 0, z), 9);
	block_set(world, vec3i(0, 0, 0), 3);
}

static void	draw_debug(t_tsr *tsr)
{
	char	str[256];

	snprintf(str, 256, "FPS \t%.2f\nPOS \t[%.1f %.1f %.1f]\n"
		"ROT \t[%.2f %.2f]\nFW  \t[%.1f %.1f %.1f]",
		1.0 / tsr->mbx->dt, tsr->player.position.x, tsr->player.position.y,
		tsr->player.position.z,
		tsr->camera.rotation.x, tsr->camera.rotation.y,
		tsr->camera.forward.x, tsr->camera.forward.y, tsr->camera.forward.z);
	mbx_set_text_scaled(tsr->mbx->vp, str,
		vec2ix2_xy(5, 5, 2, 2), tsr->ui.fonts.small);
}

static void draw_crosshair(t_mbx *mbx)
{
	t_mbx_color	color;
	t_vec2i		pos;
	int			x;
	int			y;

	pos = vec2i_div_d(mbx->viewport->size, 2);
	x = -1;
	while (x < 1)
	{
		y = -1;
		while (y < 1)
		{
			color = mbx_get_pixel(mbx->viewport, vec2i_add(pos, vec2i(x, y)));
			color = color_r_g_b_a(
				255 - color.r, 255 - color.g, 255 - color.b, 255);
			mbx_set_pixel(mbx->viewport, vec2i_add(pos, vec2i(x, y)), color);
			y++;
		}
		x++;
	}
}

static void draw_hotbar(t_tsr *tsr) {
	int			i;
	int			centered_x;
	int			bottom_y;
	int			selection_x;
	int			tile_width;

	centered_x = (tsr->mbx->viewport->size.x - tsr->ui.gui.hotbar->size.x) / 2;
	bottom_y = tsr->mbx->viewport->size.y - tsr->ui.gui.hotbar->size.y;
	selection_x = (centered_x - 1) + ((tsr->player.tile_id - 1) * (tsr->ui.gui.hotbar->size.x - 1) / 9);
	tile_width = tsr->ui.gui.hotbar->size.x / 9;
	mbx_set_region(tsr->mbx->vp, tsr->ui.gui.hotbar, vec2i(centered_x, bottom_y));
	i = 1;
	while (i <= TILE_COUNT) {
		mbx_set_subregion(tsr->mbx->vp, tsr->world.tiles[i].region[0],
			vec2i(centered_x + ((i - 1) * tile_width) + (tile_width - 16) / 2 + 1,
				bottom_y + (tsr->ui.gui.hotbar->size.y - 16) /2),
			vec2ix2_xy(0, 0, 16, 16));
		i++;
	}
	mbx_set_region(tsr->mbx->vp, tsr->ui.gui.hotbar_selection, vec2i(selection_x, bottom_y - 1));
}

static void	prepare_next_render(t_tsr *tsr)
{
	t_mbx_region	*tmp;

	tsr->rendering.current_job = 0;
	tsr->rendering.job_region_count = vec2i(
			tsr->mbx->vp->size.x / RENDER_JOB_REGION_W,
			tsr->mbx->vp->size.y / RENDER_JOB_REGION_H);
	tsr->rendering.job_count = (tsr->rendering.job_region_count.x
			* tsr->rendering.job_region_count.y);
	tmp = tsr->rendering.target;
	tsr->rendering.target = tsr->mbx->viewport;
	tsr->mbx->viewport = tmp;
}

void	update(t_mbx *mbx, void *data)
{
	t_tsr	*tsr;
	double cosa = 0.999998477;
	double sina = 0.001745328;

	(void)mbx;
	tsr = data;
	tsr_update_camera(tsr);
	tsr_update_player(tsr);
	tsr->world.global_light = vec3(
		tsr->world.global_light.x * cosa - tsr->world.global_light.z * sina,
		tsr->world.global_light.y,
		tsr->world.global_light.x * sina + tsr->world.global_light.z * cosa);
	if (mbx_btnp(tsr->mbx, MBX_KEY_F1))
	{
		tsr->mbx->settings.lock_cursor = !tsr->mbx->settings.lock_cursor;
		tsr->mbx->settings.show_cursor = !tsr->mbx->settings.show_cursor;
		mbx_refresh_settings(tsr->mbx);
	}
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	prepare_next_render(tsr);
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	draw_debug(tsr);
	draw_crosshair(tsr->mbx);
	draw_hotbar(tsr);
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
