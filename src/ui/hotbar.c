/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotbar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 23:54:59 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/29 00:07:19 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_drawing.h"
#include "tsr.h"
#include "tsr_core.h"
#include "tsr_utils.h"

static void	set_hotbar_offset(t_tsr *tsr)
{
	const double	diff = tsr->player.hotbar_tile - tsr->ui.hotbar.offset;
	double			delta;

	if (fabs(diff) < tsr->world.data->tile_count / 2.)
		delta = 0.2 * diff;
	else
		delta = 0.2 * fsign(diff) * (fabs(diff) - tsr->world.data->tile_count);
	if (fabs(delta) < 0.01)
	{
		tsr->ui.hotbar.offset = tsr->player.hotbar_tile;
		return ;
	}
	tsr->ui.hotbar.delta += delta;
	tsr->ui.hotbar.delta *= 0.5;
	tsr->ui.hotbar.offset = fwrap(
			tsr->ui.hotbar.offset + tsr->ui.hotbar.delta,
			1, tsr->world.data->tile_count);
}

void	draw_hotbar(t_tsr *tsr)
{
	const char			*name
		= tsr->world_data.tiles[tsr->player.hotbar_tile].name;
	t_mbx_region		*texture;
	double				off;
	int					i;

	set_hotbar_offset(tsr);
	i = -6;
	while (i <= 6)
	{
		off = i - tsr->ui.hotbar.offset + (int)(tsr->ui.hotbar.offset);
		texture = tsr->world.data->tiles[wrap((int)(tsr->ui.hotbar.offset) + i,
				1, tsr->world.data->tile_count)].texture[0]->texture;
		mbx_set_subregion_scaled(tsr->ui.target, texture, vec2ix3(
				vec2i(312 + 20 * off, 338 - 20 * (cos(off * off / 22.3) - 1)),
				vec2i(0, 0), texture->subregion_size),
			vec2_div_rd(16, vec2_vi(texture->subregion_size)));
		i++;
	}
	mbx_set_region(tsr->ui.target, tsr->textures.hotbar_selection,
		vec2i(308, 334));
	mbx_set_text(tsr->ui.target, name,
		vec2i(320 - tsr_strlen(name) * 2.5, 325), tsr->textures.font_small);
}
