/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   title.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:07:04 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 18:07:04 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	shift(void *data, t_mbx_region *region, int index, t_mbx_color col)
{
	t_mbx	*mbx;

	mbx = data;
	col.r = !!(~(uint8_t)mbx->now & 0b110000) * 255;
	col.g = !!(~(uint8_t)mbx->now & 0b1100) * 255;
	col.b = !!(~(uint8_t)mbx->now & 0b11) * 255;
	region->pixels[index] = col;
}

static t_mbx_color	reverse(void *data, t_mbx_region *region, int index)
{
	(void)data;
	return (color_rgba(~(region->pixels[index].rgba & ~0xFF)));
}

static void	set_wierd_char(t_tsr *tsr, char c, t_vec2i pos, t_vec2 scale)
{
	const uint8_t	mask = (((int)(
			(tsr->mbx->now - tsr->mbx->app_timestamp + ~pos.x) * 10))
			^ ~(int)tsr->mbx->now) & 0b11111111;
	t_vec2ix3		pos_uv_wh;

	pos_uv_wh = vec2ix3(pos, vec2i_mult(vec2i(c % 32, c / 32),
				tsr->textures.font_title->subregion_size), vec2i(9, 10));
	if (!mask)
		return ;
	if (mask == 0b1)
		pos_uv_wh.p3.x *= -1;
	if (mask == 0b10)
		pos_uv_wh.p3.y *= -1;
	if (mask == 0b100)
		pos_uv_wh.p1.y += 10;
	if (mask == 0b1000)
		pos_uv_wh.p1.y -= 10;
	if (mask == 0b101)
		pos_uv_wh.p3.x /= 2;
	if (mask == 0b1001)
		pos_uv_wh.p3.y /= 2;
	if (mask == 0b110)
	{
		scale.y /= 2;
		pos_uv_wh.p1.y += pos_uv_wh.p3.y / 2;
	}
	if (mask == 0b10010 && pos_uv_wh.p2.y == 'A'
		/ 32 * tsr->textures.font_title->subregion_size.y)
		pos_uv_wh.p2.y += tsr->textures.font_title->subregion_size.y;
	if (mask == 0b100000)
		tsr->ui.target->pipeline.set = &shift;
	if (mask == 0b100001)
		tsr->textures.font_title->pipeline.get = &reverse;
	mbx_set_subregion_scaled(tsr->ui.target, tsr->textures.font_title,
		pos_uv_wh, scale);
	tsr->ui.target->pipeline.set = tsr->mbx->settings.default_pipeline.set;
	tsr->textures.font_title->pipeline.get
		= tsr->mbx->settings.default_pipeline.get;
}

void	draw_title(t_tsr *tsr, char *str, t_vec2i pos, t_vec2 scale)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		set_wierd_char(tsr, str[i], pos, scale);
		pos.x += tsr->textures.font_title->subregion_size.x * scale.x;
		i++;
	}
}
