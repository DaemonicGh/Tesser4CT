/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 20:33:02 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/26 20:33:02 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_inputs.h"
#include "tsr.h"

void	button_init(t_tsr_button *button, t_vec2i position, const char *message)
{
	*button = (t_tsr_button){0};
	button->position = position;
	button->message = message;
	button->visible = true;
	button->hovered = false;
	button->clicked = false;
}

bool	button_update(t_tsr *tsr, t_tsr_button *button)
{
	button->hovered = (tsr->ui.cursor.x >= button->position.x
			&& tsr->ui.cursor.y >= button->position.y
			&& tsr->ui.cursor.x <= button->position.x + 128
			&& tsr->ui.cursor.y <= button->position.y + 26);
	if (!button->hovered)
		return (false);
	if (mbx_key_released(tsr->mbx, MBX_MOUSE_LEFT))
		return (true);
	if (mbx_key_held(tsr->mbx, MBX_MOUSE_LEFT))
		button->clicked = true;
	return (false);
}

void	button_draw(t_tsr *tsr, t_tsr_button *button)
{
	const t_vec2i	scale = vec2i_i(2);
	size_t			len;

	if (!button->visible)
		return ;
	mbx_set_subregion_scaled(tsr->ui.target, tsr->textures.gui.button,
		vec2ix3(button->position, vec2i(0, (button->hovered + button->clicked)
				* tsr->textures.gui.button->subsize.y),
			tsr->textures.gui.button->subsize), vec2_vi(scale));
	len = tsr_strlen(button->message);
	mbx_set_text_scaled(tsr->ui.target, button->message,
		vec2ix2(vec2i_add(button->position, vec2i(65 - len * 5, 8)), scale),
		tsr->textures.font_small);
}
