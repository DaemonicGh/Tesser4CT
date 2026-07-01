/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 23:54:54 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 17:55:05 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

static void	draw_debug(t_tsr *tsr)
{
	char	str[32];

	snprintf(str, 32, "FPS\t\t%.2f", 1.0 / tsr->mbx->dt);
	mbx_set_text(tsr->ui.target, str, vec2i(3, 3), tsr->textures.font_small);
}

static void	draw_debug_ext(t_tsr *tsr)
{
	const t_tsr_tile	*tile = tsr_get_tile(&tsr->world,
			tsr->player.tile_highlight_chunk, tsr->player.tile_highlight_pos);
	char				str[256];

	snprintf(str, 256, "FPS\t\t%.2f\nPos\t\t[%.1f %.1f %.1f]\n"
		"Rot\t\t[%.2f %.2f] %i\nChunk\t%u\nTile\t%u[%u]",
		1.0 / tsr->mbx->dt, tsr->player.position.x,
		tsr->player.position.y, tsr->player.position.z,
		tsr->camera.rotation.x, tsr->camera.rotation.y, tsr->player.face.z,
		tsr->player.chunk, tile->type, tile->rotation);
	mbx_set_text(tsr->ui.target, str, vec2i(3, 3), tsr->textures.font_small);
}

static void	draw_crosshair(t_tsr *tsr)
{
	t_mbx_color		color;
	const t_vec2i	vp_pos = vec2i_div_d(tsr->mbx->vp->size, 2);
	const t_vec2i	ui_pos = vec2i_div_d(tsr->ui.target->size, 2);
	t_vec2i			xy;

	xy.x = -1;
	while (xy.x < 1)
	{
		xy.y = -1;
		while (xy.y < 1)
		{
			color = mbx_get_pixel(tsr->mbx->vp, vec2i_add(vp_pos, xy));
			color = color_r_g_b_a(~color.r, ~color.g, ~color.b, 255);
			mbx_set_pixel(tsr->ui.target, vec2i_add(ui_pos, xy), color);
			xy.y++;
		}
		xy.x++;
	}
}

void	draw_ui(t_tsr *tsr)
{
	draw_crosshair(tsr);
	prompt_draw(tsr);
	if (!tsr->player.prompt_state
		&& mbx_key_held(tsr->mbx, MBX_KEY_TAB))
		update_and_draw_minimap(tsr);
	if (tsr->player.godmode)
	{
		draw_debug_ext(tsr);
		draw_hotbar(tsr);
		draw_tool_hotbar(tsr);
	}
	else
		draw_debug(tsr);
}
