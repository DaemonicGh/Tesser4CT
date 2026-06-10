/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:55:36 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/21 20:49:38 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static int	inside(t_vec2i pos, t_vec2i size, t_vec2i mouse)
{
	return (pos.x < mouse.x
		&& pos.x + size.x > mouse.x
		&& pos.y < mouse.y
		&& pos.y + size.y > mouse.y);
}

void	draw_button(t_tsr *tsr, t_ui_button *button)
{
	const t_vec2i	charset_size = tsr->textures.font_small->subregion_size;
	t_mbx_color		color;
	int				scale;
	t_vec2i			text_size;

	scale = min(button->size.x / (charset_size.x * tsr_strlen(button->str)),
			button->size.y / charset_size.y);
	text_size = vec2i_mult_d(charset_size, scale);
	text_size.x *= tsr_strlen(button->str);
	color = button->still_col;
	if (button->hovered)
		color = button->hover_col;
	mbx_set_rect(tsr->ui.target, button->pos, button->size, color);
	mbx_set_text_scaled(tsr->ui.target, button->str,
		vec2ix2_xy(
			button->pos.x + (button->size.x - text_size.x) / 2,
			button->pos.y + (button->size.y - text_size.y) / 2 + scale,
			scale, scale),
		tsr->textures.font_small);
}

void	update_button(t_tsr *tsr, t_ui_button *button, void (*f)(t_tsr *))
{
	button->hovered = inside(button->pos, button->size,
			vec2i_vd(tsr->mbx->cursor));
	if (button->hovered && f)
		f(tsr);
}
