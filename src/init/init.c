/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 17:02:26 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/29 02:20:03 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

static void	init_data(t_tsr *tsr)
{
	load_regions(tsr);
	load_texture_data(tsr);
	load_tile_data(tsr);
}

static void	init_player(t_tsr *tsr)
{
	tsr->player.hitbox = vec3x2(vec3(-0.4, -1.4, -0.4), vec3(0.4, 0.4, 0.4));
	tsr->player.velocity = vec3_zero();
	tsr->player.hotbar_tile = tsr_tile(tsr, 2, 3, 0);
	tsr->player.hotbar_tool = TOOL_CREATE_CHUNK;
	tsr->player.speed = 0.12;
	tsr->player.godmode = true;
}

static void	init_values(t_tsr *tsr)
{
	tsr->mbx->settings.fps_cap = 60;
	tsr->mbx->settings.exit_key = MBX_KEY_NONE;
	tsr->mbx->settings.viewport_render = MBX_VIEWPORT_RENDER_SKIP;
	tsr->extras.aspect_ratio = (double)DEFAULT_VIEWPORT_W / DEFAULT_VIEWPORT_H;
	tsr->extras.fov = 90.0;
	tsr->extras.focal_length = 1.0;
	tsr->extras.camera_sensitivity = 2;
	tsr->extras.render_scale = 360;
	tsr->extras.render_distance = 16;
	tsr->extras.shadow_distance = 8;
	tsr->extras.skybox_distance = 8;
	tsr->extras.cross_fill = true;
	tsr->ui.state = UI_STATE_MAIN;
	tsr->ui.hotbar.offset = 1;
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
	init_data(tsr);
	init_player(tsr);
	init_values(tsr);
	init_rendering(tsr);
	mbx_center_cursor(tsr->mbx);
	return (tsr);
}
