/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 23:54:54 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/10 23:54:54 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	draw_debug(t_tsr *tsr)
{
	char	str[256];

	snprintf(str, 256, "FPS \t%.2f\nPOS \t[%.1f %.1f %.1f]\n"
		"ROT \t[%.2f %.2f]\nFW  \t[%.1f %.1f %.1f]",
		1.0 / tsr->mbx->dt, tsr->player.position.x, tsr->player.position.y,
		tsr->player.position.z,
		tsr->camera.rotation.x, tsr->camera.rotation.y,
		tsr->camera.forward.x, tsr->camera.forward.y, tsr->camera.forward.z);
	mbx_set_text(tsr->ui.target, str, vec2i(3, 3), tsr->textures.font_small);
}

static void	draw_crosshair(t_tsr *tsr)
{
	t_mbx_color	color;
	t_vec2i		pos;
	t_vec2i		xy;

	pos = vec2i_div_d(tsr->mbx->vp->size, 2);
	xy.x = -1;
	while (xy.x < 1)
	{
		xy.y = -1;
		while (xy.y < 1)
		{
			color = mbx_get_pixel(tsr->mbx->vp, vec2i_add(pos, xy));
			color = color_r_g_b_a(~color.r, ~color.g, ~color.b, 255);
			mbx_set_pixel(tsr->ui.target, vec2i_add(pos, xy), color);
			xy.y++;
		}
		xy.x++;
	}
}

void	draw_ui(t_tsr *tsr)
{
	draw_debug(tsr);
	draw_crosshair(tsr);
	draw_hotbar(tsr);
}
