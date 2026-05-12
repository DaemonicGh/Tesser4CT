/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 02:42:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 02:42:42 by rprieur          ###   ########.fr       */
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
}

void	tsr_draw_game(t_tsr *tsr)
{
	draw_ui(tsr);
	mbx_render_region_as_viewport(tsr->mbx, tsr->mbx->vp,
		MBX_VIEWPORT_RENDER_KEEP);
	mbx_render_region_as_viewport(tsr->mbx, tsr->ui.target,
		MBX_VIEWPORT_RENDER_KEEP);
}
