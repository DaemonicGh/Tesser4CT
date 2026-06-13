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

static void	init_data(t_tsr *tsr)
{
	load_regions(tsr);
	load_texture_data(tsr);
	load_tile_data(tsr);
}

static void	init_player(t_tsr *tsr)
{
	tsr->player.hitbox = vec3(0.4, 0.9, 0.4);
	tsr->player.velocity = vec3_zero();
	tsr->player.hotbar_tile = 2;
}

static void	init_values(t_tsr *tsr)
{
	tsr->mbx->settings.fps_cap = 60;
	tsr->mbx->settings.exit_key = MBX_KEY_NONE;
	tsr->mbx->settings.viewport_render = MBX_VIEWPORT_RENDER_SKIP;
	tsr->extras.aspect_ratio = (double)DEFAULT_VIEWPORT_H / DEFAULT_VIEWPORT_W;
	tsr->extras.fov = 90.0;
	tsr->extras.focal_length = 1.0;
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
	return (tsr);
}
