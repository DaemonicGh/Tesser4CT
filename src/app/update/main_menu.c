/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:54:27 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/28 22:40:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_inputs.h"
#include "modules/mbx_structs.h"
#include "tsr.h"

static t_mbx_color	draw_main_menu_bg(t_tsr *tsr, t_vec2 uv)
{
	const t_vec2	cursor = vec2(
			((double)tsr->mbx->cursor.x / tsr->mbx->vp->size.x - 0.5) / 8,
			((double)tsr->mbx->cursor.y / tsr->mbx->vp->size.y - 0.5) / 8);
	const t_vec2	uvc = vec2(
			(uv.x - 0.5 + cursor.x) * 16 / 9, uv.y - 0.5 + cursor.y);
	const double	dist = (fabs(uvc.x) + fabs(uvc.y));
	const t_vec2	pos = vec2_rotate(uvc, dist * 8 - tsr->mbx->now * 2);
	t_mbx_color		col;

	col.r = (sin(dist - tsr->mbx->now * 0.4123
				+ pos.x * 1.01 + pos.y * 1.21)) * 0xFF;
	col.g = (sin(dist - tsr->mbx->now * 0.7627
				+ pos.x * 1.78 + pos.y * 0.81)) * 0xFF;
	col.b = (sin(dist - tsr->mbx->now * 0.912
				+ pos.x * 0.912 + pos.y * 0.147)) * 0xFF;
	if (col.r < 0xD)
		col.r = (0xD - col.r) * 0x10;
	if (col.g < 0xD)
		col.g = (0xD - col.g) * 0x10;
	if (col.b < 0xD)
		col.b = (0xD - col.b) * 0x10;
	return (color_r_g_b_a(col.r * 0.8, col.g * 0.8, col.b * 0.8, 0xFF));
}

static void	draw_main_menu(t_tsr *tsr)
{
	char	str[32];

	snprintf(str, 32, "FPS \t%.1f\n", 1. / tsr->mbx->dt);
	mbx_set_text(tsr->ui.target, str,
		vec2i(3, 3), tsr->textures.font_small);
	draw_title(tsr, "TESSER4CT",
		vec2i(tsr->ui.target->size.x / 2 - 180, 50), vec2(4, 4));
	mbx_set_text_scaled(tsr->ui.target, "[Press any key]",
		vec2ix2_xy(tsr->ui.target->size.x / 2 - 80, 150, 2, 2),
		tsr->textures.font_small);
	mbx_set_text_scaled(tsr->ui.target, "[X] Exit",
		vec2ix2_xy(tsr->ui.target->size.x / 2 - 40, 320, 2, 2),
		tsr->textures.font_small);
}

void	tsr_init_main_menu(t_tsr *tsr)
{
	tsr->mbx->settings.hide_cursor = false;
	tsr->mbx->settings.lock_cursor = false;
	mbx_center_cursor(tsr->mbx);
	tsr->rendering.frag_shader = draw_main_menu_bg;
}

void	tsr_update_main_menu(t_tsr *tsr)
{
	mbx_clear(tsr->ui.target, color_rgba(0x0));
	draw_main_menu(tsr);
	if (mbx_key_released(tsr->mbx, MBX_KEY_ESCAPE)
		|| mbx_key_released(tsr->mbx, MBX_KEY_X))
		tsr->mbx->exiting = true;
	else if (tsr->mbx->last_release == 0
		|| mbx_key_released(tsr->mbx, MBX_KEY_SPACE))
		tsr->ui.state = UI_STATE_GAME;
}
