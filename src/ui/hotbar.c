/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotbar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 23:54:59 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/10 23:54:59 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	set_hotbar_offset(t_tsr *tsr)
{
	const double	diff = tsr->player.tile_id - tsr->ui.hotbar.offset;
	double			delta;

	if (fabs(diff) < TILE_COUNT / 2.)
		delta = 0.2 * diff;
	else
		delta = 0.2 * fsign(diff) * (fabs(diff) - TILE_COUNT);
	if (fabs(delta) < 0.01)
	{
		tsr->ui.hotbar.offset = tsr->player.tile_id;
		return ;
	}
	tsr->ui.hotbar.delta += delta;
	tsr->ui.hotbar.delta *= 0.5;
	tsr->ui.hotbar.offset = fwrap(
			tsr->ui.hotbar.offset + tsr->ui.hotbar.delta,
			1, TILE_COUNT + 1);
}

void	draw_hotbar(t_tsr *tsr)
{
	t_vec2i				pos;
	double				off;
	int					i;

	pos = vec2i(tsr->mbx->viewport->size.x / 2,
			tsr->mbx->viewport->size.y - 22);
	mbx_set_rect(tsr->ui.target, vec2i(pos.x - 135, pos.y - 8),
		vec2i(270, 30), color_rgba(0x9966FF22));
	set_hotbar_offset(tsr);
	i = -6;
	while (i <= 6)
	{
		off = i - tsr->ui.hotbar.offset + (int)(tsr->ui.hotbar.offset);
		mbx_set_subregion(tsr->ui.target, tsr->world.tiles[
			wrap((int)(tsr->ui.hotbar.offset) + i, 1, TILE_COUNT + 1)]
			.pbr[0].col_tex, vec2i(pos.x - 8 + 20 * off,
				pos.y - 20 * (cos(off * off / 22.3) - 1)),
			vec2ix2_xy(0, 0, 16, 16));
		i++;
	}
	mbx_set_region(tsr->ui.target, tsr->textures.hotbar_selection,
		vec2i(pos.x - 12, pos.y - 4));
}
