/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pause.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 17:49:09 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/21 20:49:38 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include <stdlib.h>

t_mbx_color	pause_background(t_tsr *tsr, t_vec2i frag_pos)
{
	const t_tsr_camera	camera = tsr->rendering.data.camera;
	const t_vec2		uvc = vec2(16. / 9
			* ((double)frag_pos.x / tsr->rendering.target->size.x - 0.5),
			(double)frag_pos.y / tsr->rendering.target->size.y - 0.5);
	const t_vec2		uvcr = vec2_add(uvc, vec2(
				cos(tsr->mbx->now + frag_pos.x * 2) * 0.01,
				sin(tsr->mbx->now + frag_pos.y * 2) * 0.01));
	t_tsr_ray			ray;

	ray = setup_ray(tsr, camera.position, camera.chunk,
			vec3_add(vec3_mult_d(camera.forward, tsr->extras.focal_length),
				vec3_add(vec3_mult_d(camera.right, uvcr.x),
					vec3_mult_d(camera.up, -uvcr.y))));
	while (ray.lifetime)
	{
		trace_ray(tsr, &ray, false);
		if (set_ray_tile_color(tsr, &ray, frag_pos))
			break ;
	}
	return (ray.color);
}

static void	draw_pause_menu(t_tsr *tsr)
{
	char	str[32];

	snprintf(str, 32, "FPS \t%.1f\n", 1.0 / tsr->mbx->dt);
	mbx_set_text(tsr->ui.target, str, vec2i(3, 3), tsr->textures.font_small);
	draw_title(tsr, "PAUSED",
		vec2i(tsr->ui.target->size.x / 2 - 95, 50), vec2(3, 3));
	button_draw(tsr, &tsr->ui.buttons[0]);
	button_draw(tsr, &tsr->ui.buttons[1]);
	button_draw(tsr, &tsr->ui.buttons[2]);
}

void	tsr_init_pause_menu(t_tsr *tsr)
{
	tsr->mbx->settings.hide_cursor = false;
	tsr->mbx->settings.lock_cursor = false;
	mbx_refresh_settings(tsr->mbx);
	tsr->rendering.frag_shader = pause_background;
	button_init(&tsr->ui.buttons[0], vec2i(256, 140), "Back");
	button_init(&tsr->ui.buttons[1], vec2i(256, 190), "Options...");
	button_init(&tsr->ui.buttons[2], vec2i(256, 240), "Exit");
}

void	tsr_update_pause_menu(t_tsr *tsr)
{
	update_light(tsr);
	mbx_clear(tsr->ui.target, color_rgba(0x00000044));
	draw_pause_menu(tsr);
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_ESCAPE)
		|| button_update(tsr, &tsr->ui.buttons[0]))
		tsr->ui.state = UI_STATE_GAME;
	else if (button_update(tsr, &tsr->ui.buttons[1]))
		tsr->ui.state = UI_STATE_SETTING;
	else if (button_update(tsr, &tsr->ui.buttons[2]))
		tsr->ui.state = UI_STATE_MAIN;
}
