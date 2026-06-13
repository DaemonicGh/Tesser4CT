/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 02:30:51 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/10 02:30:51 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	prompt_init(t_tsr *tsr, t_vec2i	position, const char *message)
{
	size_t	i;

	i = 0;
	while (i < PROMPT_SIZE)
		tsr->ui.prompt.buffer[i++] = 0;
	tsr->ui.prompt.visible = true;
	tsr->ui.prompt.cursor = 0;
	tsr->ui.prompt.position = position;
	tsr->ui.prompt.shake = 0;
	tsr->ui.prompt.message = message;
	tsr->ui.prompt.is_error = false;
}

bool	prompt_update(t_tsr *tsr)
{
	size_t	i;
	char	c;

	tsr->ui.prompt.shake = fmax(tsr->ui.prompt.shake - tsr->mbx->dt, 0);
	i = 0;
	while (tsr->mbx->text_input[i])
	{
		c = tsr->mbx->text_input[i++];
		if (c == '\n')
			return (true);
		else if (c == '\b')
		{
			if (tsr->ui.prompt.cursor > 0)
			{
				tsr->ui.prompt.buffer[--tsr->ui.prompt.cursor] = 0;
				tsr->ui.prompt.is_error = false;
			}
		}
		else if (c != '\t' && tsr->ui.prompt.cursor < PROMPT_SIZE - 1)
			tsr->ui.prompt.buffer[tsr->ui.prompt.cursor++] = c;
	}
	return (false);
}

void	prompt_draw(t_tsr *tsr)
{
	const t_vec2i	scale = vec2i(2, 2);
	const t_vec2i	pos = vec2i_add(tsr->ui.prompt.position, vec2i(
				sin(tsr->ui.prompt.shake * 64) * tsr->ui.prompt.shake * 4, 0));
	const t_vec2ix3	ele_pos = vec2ix3(
			vec2i_add(tsr->ui.prompt.position, vec2i(0, -20)),
			vec2i_add(tsr->ui.prompt.position, vec2i(0, 20)),
			vec2i_add(pos, vec2i(-4, -4)));

	if (!tsr->ui.prompt.visible)
		return ;
	mbx_set_text_scaled(tsr->ui.target, tsr->ui.prompt.message,
		vec2ix2(ele_pos.p1, scale), tsr->textures.font_small);
	if (tsr->ui.prompt.is_error)
	{
		mbx_set_rect(tsr->ui.target, ele_pos.p3,
			vec2i(166, 18), color_rgba(0xAA0000BB));
	}
	else
		mbx_set_rect(tsr->ui.target, ele_pos.p3,
			vec2i(166, 18), color_rgba(0xBB));
	mbx_set_text_scaled(tsr->ui.target,
		tsr->ui.prompt.buffer + max(tsr->ui.prompt.cursor - 16, 0),
		vec2ix2(pos, scale), tsr->textures.font_small);
}
