/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pause.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 17:49:09 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 17:49:09 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	draw_pause_menu(t_tsr *tsr)
{
	char	str[32];

	snprintf(str, 32, "FPS \t%.1f\n", 1.0 / tsr->mbx->dt);
	mbx_set_text(tsr->ui.target, str, vec2i(3, 3), tsr->textures.font_small);
	draw_title(tsr, "PAUSED",
		vec2i(tsr->ui.target->size.x / 2 - 95, 50), vec2(3, 3));
	mbx_set_text_scaled(tsr->ui.target,
		"[ESC] Back to Game\n[X]   Exit",
		vec2ix2_xy(tsr->ui.target->size.x / 2 - 100, 120, 2, 2),
		tsr->textures.font_small);
}

void	tsr_init_pause_menu(t_tsr *tsr)
{
	tsr->mbx->settings.show_cursor = true;
	tsr->mbx->settings.lock_cursor = false;
	mbx_refresh_settings(tsr->mbx);
	mbx_center_cursor(tsr->mbx);
	tsr->rendering.frag_shader = draw_ray;
}

void	tsr_update_pause_menu(t_tsr *tsr)
{
	mbx_clear(tsr->ui.target, color_rgba(0x00000066));
	draw_pause_menu(tsr);
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_ESCAPE))
		tsr->ui.state = UI_STATE_GAME;
	else if (mbx_key_released(tsr->mbx, MBX_KEY_X))
		tsr->ui.state = UI_STATE_MAIN;
}
