/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 15:50:34 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/25 15:50:34 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	slider_init(t_tsr_slider *slider,
	t_vec2i position, const char *message, t_vec4 value_bounds_step)
{
	*slider = (t_tsr_slider){0};
	slider->position = position;
	slider->message = message;
	slider->value = value_bounds_step.x;
	slider->bounds[0] = value_bounds_step.y;
	slider->bounds[1] = value_bounds_step.z;
	slider->step = value_bounds_step.w;
	slider->hovered = false;
	slider->visible = true;
	slider->value = fclamp(
			round(slider->value / slider->step) * slider->step,
			slider->bounds[0], slider->bounds[1]);
}

double	slider_update(t_tsr *tsr, t_tsr_slider *slider)
{
	if (!mbx_key_held(tsr->mbx, MBX_MOUSE_LEFT))
		slider->hovered = (tsr->ui.cursor.x >= slider->position.x
				&& tsr->ui.cursor.y >= slider->position.y
				&& tsr->ui.cursor.x <= slider->position.x + 178
				&& tsr->ui.cursor.y <= slider->position.y + 26);
	if (!slider->hovered)
		return (slider->value);
	if (mbx_key_held(tsr->mbx, MBX_MOUSE_LEFT))
		slider->value = flerp(slider->bounds[0], slider->bounds[1],
				inv_flerp(slider->position.x, slider->position.x + 178,
					tsr->ui.cursor.x));
	slider->value += slider->step * tsr->mbx->scroll_delta;
	slider->value = fclamp(
			round(slider->value / slider->step) * slider->step,
			slider->bounds[0], slider->bounds[1]);
	return (slider->value);
}

void	slider_draw(t_tsr *tsr, t_tsr_slider *slider)
{
	const t_vec2i	scale = vec2i_i(2);
	int				cursor_position;
	char			message[64];

	if (!slider->visible)
		return ;
	cursor_position = (
			lerp(slider->position.x + 6, slider->position.x + 162,
				inv_flerp(slider->bounds[0], slider->bounds[1], slider->value))
			/ 2) * 2 + 1;
	mbx_set_subregion_scaled(tsr->ui.target, tsr->textures.gui.bar,
		vec2ix3(slider->position, vec2i(0, 0),
			tsr->textures.gui.bar->subsize), vec2_vi(scale));
	mbx_set_subregion_scaled(tsr->ui.target, tsr->textures.gui.slider,
		vec2ix3(vec2i(cursor_position, slider->position.y),
			vec2i(tsr->textures.gui.slider->subsize.x * slider->hovered, 0),
			tsr->textures.gui.slider->subsize), vec2_vi(scale));
	snprintf(message, 64, "%s: %.6g", slider->message, slider->value);
	mbx_set_text_scaled(tsr->ui.target, message,
		vec2ix2(vec2i_add_d(slider->position, 8), scale),
		tsr->textures.font_small);
}
