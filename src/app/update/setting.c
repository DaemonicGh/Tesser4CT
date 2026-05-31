/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:15:41 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/21 20:49:38 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	light_red_channel(t_tsr *tsr)
{
	tsr->world.global_light_col.x += (double)tsr->mbx->scroll_delta * 0.05;
}

static void	light_green_channel(t_tsr *tsr)
{
	tsr->world.global_light_col.y += (double)tsr->mbx->scroll_delta * 0.05;
}

static void	light_blue_channel(t_tsr *tsr)
{
	tsr->world.global_light_col.z += (double)tsr->mbx->scroll_delta * 0.05;
}

static void	fov_setting(t_tsr *tsr)
{
	tsr->extras.fov += (double)tsr->mbx->scroll_delta;
	tsr->extras.focal_length = 1.0 / tan(tsr->extras.fov * 0.5 * M_PI / 180);
}

void	draw_setting_menu(t_tsr *tsr, t_ui_button *button)
{
	char	str[16];

	draw_title(tsr, "SETTINGS",
		vec2i(tsr->ui.target->size.x / 2 - 120, 50), vec2(3, 3));
	snprintf(str, 16, "Light R: %.2f", tsr->world.global_light_col.x);
	button[0].str = str;
	draw_button(tsr, button + 0);
	snprintf(str, 16, "Light G: %.2f", tsr->world.global_light_col.y);
	button[1].str = str;
	draw_button(tsr, button + 1);
	snprintf(str, 16, "Light B: %.2f", tsr->world.global_light_col.z);
	button[2].str = str;
	draw_button(tsr, button + 2);
	snprintf(str, 16, "FOV: %.2f", tsr->extras.fov);
	button[3].str = str;
	draw_button(tsr, button + 3);
}

void	tsr_init_setting_menu(t_tsr *tsr)
{
	tsr->mbx->settings.show_cursor = true;
	tsr->mbx->settings.lock_cursor = false;
	mbx_refresh_settings(tsr->mbx);
	tsr->rendering.frag_shader = draw_ray;
}

void	tsr_update_setting_menu(t_tsr *tsr)
{
	t_ui_button	button[4];
	const int	center = tsr->mbx->vp->size.x / 2;

	button[0] = (t_ui_button){
		vec2i(center - 70, 120), vec2i(140, 21), NULL,
		color_rgba(0xff6060ff), color_rgba(0xff8080ff), false};
	button[1] = (t_ui_button){
		vec2i(center - 70, 145), vec2i(140, 21), NULL,
		color_rgba(0x60ff60ff), color_rgba(0x80ff80ff), false};
	button[2] = (t_ui_button){
		vec2i(center - 70, 170), vec2i(140, 21), NULL,
		color_rgba(0x6060ffff), color_rgba(0x8080ffff), false};
	button[3] = (t_ui_button){
		vec2i(center - 70, 195), vec2i(140, 21), NULL,
		color_rgba(0x606060ff), color_rgba(0x808080ff), false};
	mbx_clear(tsr->ui.target, color_rgba(0x00000066));
	update_button(tsr, button + 0, light_red_channel);
	update_button(tsr, button + 1, light_green_channel);
	update_button(tsr, button + 2, light_blue_channel);
	update_button(tsr, button + 3, fov_setting);
	draw_setting_menu(tsr, button);
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_ESCAPE))
		tsr->ui.state = UI_STATE_PAUSE;
}
