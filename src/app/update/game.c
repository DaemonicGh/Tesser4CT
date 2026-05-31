/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 02:42:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/20 16:28:23 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	update_light(t_tsr *tsr)
{
	const double	cosa = 0.999998477;
	const double	sina = 0.001745328;

	tsr->world.global_light = vec3(
			tsr->world.global_light.x * cosa
			- tsr->world.global_light.z * sina,
			tsr->world.global_light.y,
			tsr->world.global_light.x * sina
			+ tsr->world.global_light.z * cosa);
}

void	tsr_init_game(t_tsr *tsr)
{
	tsr->mbx->settings.show_cursor = false;
	tsr->mbx->settings.lock_cursor = true;
	tsr->rendering.frag_shader = draw_ray;
}

void	tsr_update_game(t_tsr *tsr)
{
	tsr_update_player(tsr);
	update_light(tsr);
	if (mbx_btnp(tsr->mbx, MBX_KEY_F1))
	{
		tsr->mbx->settings.lock_cursor = !tsr->mbx->settings.lock_cursor;
		tsr->mbx->settings.show_cursor = !tsr->mbx->settings.show_cursor;
		mbx_refresh_settings(tsr->mbx);
	}
	if (mbx_btnp(tsr->mbx, MBX_KEY_ESCAPE))
		tsr->ui.state = UI_STATE_PAUSE;
	mbx_clear(tsr->ui.target, color_rgba(0x0));
	draw_ui(tsr);
}
