/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:15:38 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/21 20:49:38 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_handlers.h"
#include "tsr.h"
#include "tsr_core.h"
#include "tsr_ui.h"

static void	draw_setting_menu(t_tsr *tsr)
{
	int	i;

	mbx_set_rect(tsr->ui.target,
		vec2i(32, 104), vec2i(190, 228), color_rgba(0x773344BB));
	mbx_set_rect(tsr->ui.target,
		vec2i(225, 104), vec2i(190, 188), color_rgba(0x226633BB));
	mbx_set_rect(tsr->ui.target,
		vec2i(418, 104), vec2i(190, 228), color_rgba(0x553377BB));
	draw_title(tsr, "SETTINGS",
		vec2i(tsr->ui.target->size.x / 2 - 120, 50), vec2(3, 3));
	mbx_set_text_scaled(tsr->ui.target, "Raytracing\nDistances",
		vec2ix2_xy(231, 110, 2, 2), tsr->textures.font_small);
	mbx_set_text_scaled(tsr->ui.target, "WORLD SETTINGS",
		vec2ix2_xy(424, 110, 2, 2), tsr->textures.font_small);
	mbx_set_text_scaled(tsr->ui.target, "Ambient Color",
		vec2ix2_xy(424, 134, 2, 2), tsr->textures.font_small);
	i = 0;
	while (i < 12)
		slider_draw(tsr, &tsr->ui.sliders[i++]);
	toggle_draw(tsr, &tsr->ui.toggles[0]);
	toggle_draw(tsr, &tsr->ui.toggles[1]);
	button_draw(tsr, &tsr->ui.buttons[0]);
}

static void	init_sliders(t_tsr *tsr, t_tsr_slider *sliders)
{
	slider_init(&sliders[0], vec2i(38, 110), "FPS",
		vec4(tsr->mbx->settings.fps_cap, 10, 120, 10));
	slider_init(&sliders[1], vec2i(38, 140), "Resolution",
		vec4(tsr->extras.render_scale, 90, tsr->mbx->win.size.y, 90));
	slider_init(&sliders[2], vec2i(38, 170), "FOV",
		vec4(tsr->extras.fov, 60, 120, 5));
	slider_init(&sliders[3], vec2i(38, 200), "Sensitivity",
		vec4(tsr->extras.camera_sensitivity, 0.5, 4, 0.1));
	slider_init(&sliders[4], vec2i(231, 140), "Rendering",
		vec4(tsr->extras.render_distance, 1, 32, 1));
	slider_init(&sliders[5], vec2i(231, 170), "Shadows",
		vec4(tsr->extras.shadow_distance, 1, 32, 1));
	slider_init(&sliders[6], vec2i(231, 200), "Skyboxes",
		vec4(tsr->extras.skybox_distance, 1, 32, 1));
	slider_init(&sliders[8], vec2i(424, 150), "R",
		vec4(tsr->world_data.ambient_color.r, 0, 1, 0.01));
	slider_init(&sliders[9], vec2i(424, 176), "G",
		vec4(tsr->world_data.ambient_color.g, 0, 1, 0.01));
	slider_init(&sliders[10], vec2i(424, 202), "B",
		vec4(tsr->world_data.ambient_color.b, 0, 1, 0.01));
}

void	tsr_init_setting_menu(t_tsr *tsr)
{
	tsr->mbx->settings.hide_cursor = false;
	tsr->mbx->settings.lock_cursor = false;
	mbx_refresh_settings(tsr->mbx);
	tsr->rendering.frag_shader = pause_background;
	init_sliders(tsr, tsr->ui.sliders);
	toggle_init(&tsr->ui.toggles[0], vec2i(38, 235),
		"Cross Fill", tsr->extras.cross_fill);
	toggle_init(&tsr->ui.toggles[1], vec2i(38, 260),
		"Toggle Toolbar", tsr->extras.toggle_toolbar);
	button_init(&tsr->ui.buttons[0], vec2i(256, 300), "Back");
}

static void	update_sliders(t_tsr *tsr)
{
	int			value;

	value = slider_update(tsr, &tsr->ui.sliders[0]);
	if (value != tsr->mbx->settings.fps_cap)
	{
		tsr->mbx->settings.fps_cap = value;
		mbx_refresh_settings(tsr->mbx);
	}
	tsr->extras.render_scale = min(tsr->mbx->win.size.y,
			slider_update(tsr, &tsr->ui.sliders[1]));
	tsr->extras.fov = slider_update(tsr, &tsr->ui.sliders[2]);
	tsr->extras.focal_length = 1.0 / tan(tsr->extras.fov * M_PI_2 / 180);
	tsr->extras.camera_sensitivity = slider_update(tsr, &tsr->ui.sliders[3]);
	tsr->extras.render_distance = slider_update(tsr, &tsr->ui.sliders[4]);
	tsr->extras.shadow_distance = slider_update(tsr, &tsr->ui.sliders[5]);
	tsr->extras.skybox_distance = slider_update(tsr, &tsr->ui.sliders[6]);
	tsr->world_data.ambient_color.r = slider_update(tsr, &tsr->ui.sliders[8]);
	tsr->world_data.ambient_color.g = slider_update(tsr, &tsr->ui.sliders[9]);
	tsr->world_data.ambient_color.b = slider_update(tsr, &tsr->ui.sliders[10]);
}

void	tsr_update_setting_menu(t_tsr *tsr)
{
	update_light(tsr);
	mbx_clear(tsr->ui.target, color_rgba(0x00000066));
	update_sliders(tsr);
	tsr->extras.cross_fill = toggle_update(tsr, &tsr->ui.toggles[0]);
	tsr->extras.toggle_toolbar = toggle_update(tsr, &tsr->ui.toggles[1]);
	draw_setting_menu(tsr);
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_ESCAPE)
		|| button_update(tsr, &tsr->ui.buttons[0]))
		tsr->ui.state = UI_STATE_PAUSE;
}
