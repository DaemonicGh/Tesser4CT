/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkbox.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 17:42:09 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/26 17:42:09 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	toggle_init(t_tsr_toggle *toggle,
	t_vec2i position, const char *message, bool value)
{
	*toggle = (t_tsr_toggle){0};
	toggle->position = position;
	toggle->message = message;
	toggle->value = value;
	toggle->hovered = false;
	toggle->visible = true;
}

bool	toggle_update(t_tsr *tsr, t_tsr_toggle *toggle)
{
	toggle->hovered = (tsr->ui.cursor.x >= toggle->position.x
			&& tsr->ui.cursor.y >= toggle->position.y
			&& tsr->ui.cursor.x <= toggle->position.x + 28
			&& tsr->ui.cursor.y <= toggle->position.y + 16);
	if (!toggle->hovered)
		return (toggle->value);
	if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_LEFT))
		toggle->value = !toggle->value;
	else if (tsr->mbx->scroll_delta > 0)
		toggle->value = true;
	else if (tsr->mbx->scroll_delta < 0)
		toggle->value = false;
	return (toggle->value);
}

void	toggle_draw(t_tsr *tsr, t_tsr_toggle *toggle)
{
	const t_vec2i	scale = vec2i_i(2);
	t_vec2i			wh;

	if (!toggle->visible)
		return ;
	wh = tsr->textures.gui.toggle->subsize;
	if (!toggle->value)
		wh.x = -wh.x;
	mbx_set_subregion_scaled(tsr->ui.target, tsr->textures.gui.toggle,
		vec2ix3(toggle->position, vec2i(0, toggle->hovered * wh.y), wh),
		vec2_vi(scale));
	mbx_set_text_scaled(tsr->ui.target, toggle->message,
		vec2ix2(vec2i_add(toggle->position, vec2i(32, 2)), scale),
		tsr->textures.font_small);
}
